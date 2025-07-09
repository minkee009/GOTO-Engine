import re
import sys
import os
from pathlib import Path

# 파라미터 타입 정규화 함수
def normalize_parameter_type(param_str):
    """파라미터 문자열을 정규화하여 타입을 추출"""
    if not param_str:
        return ""
    
    # 공백 정리
    param_str = param_str.strip()
    
    # const 제거
    param_str = re.sub(r'\bconst\b\s*', '', param_str)
    
    # 변수명 제거 (타입만 남기기)
    # 포인터나 참조가 있는 경우를 고려
    # 예: "int* other" -> "int*", "const char* name" -> "char*"
    match = re.match(r'(.+?)(?:\s+\w+)?$', param_str)
    if match:
        type_part = match.group(1).strip()
        # 공백 정리 (int * -> int*)
        type_part = re.sub(r'\s*\*\s*', '*', type_part)
        type_part = re.sub(r'\s*&\s*', '&', type_part)
        return type_part
    
    return param_str

# 파라미터 리스트 파싱 함수
def parse_parameters(params_str):
    """파라미터 문자열을 파싱하여 타입 리스트 반환"""
    if not params_str.strip():
        return []
    
    # 쉼표로 분할하되, 템플릿 안의 쉼표는 무시
    params = []
    current_param = ""
    bracket_count = 0
    angle_count = 0
    
    for char in params_str:
        if char == '<':
            angle_count += 1
        elif char == '>':
            angle_count -= 1
        elif char == '(':
            bracket_count += 1
        elif char == ')':
            bracket_count -= 1
        elif char == ',' and bracket_count == 0 and angle_count == 0:
            if current_param.strip():
                params.append(normalize_parameter_type(current_param))
            current_param = ""
            continue
        
        current_param += char
    
    # 마지막 파라미터 처리
    if current_param.strip():
        params.append(normalize_parameter_type(current_param))
    
    return params

# 시그니처 목록 로드
def load_signatures(signatures_path):
    signatures = {}
    with open(signatures_path, encoding="utf-8") as f:
        for line in f:
            line = line.strip()
            if not line:
                continue
            match = re.match(r'void\s+(\w+)\s*\((.*)\)', line)
            if match:
                name = match.group(1)
                params_str = match.group(2).strip()
                param_list = parse_parameters(params_str)
                signatures[name] = param_list
    return signatures

# 클래스 이름 추출
def extract_class_name(header_text):
    match = re.search(r'class\s+(\w+)\s*:\s*public\s+\w+', header_text)
    if match:
        return match.group(1)
    raise Exception("클래스 이름을 찾을 수 없습니다.")

# 헤더에서 함수 목록 추출 (빈 구현체도 포함)
def extract_functions(header_text):
    # 함수 선언과 빈 구현체 모두 매칭
    patterns = [
        r'void\s+(\w+)\s*\(([^)]*)\)\s*;',  # 함수 선언
        r'void\s+(\w+)\s*\(([^)]*)\)\s*\{[^}]*\}'  # 함수 구현체 (빈 구현체 포함)
    ]
    
    result = []
    found_functions = set()  # 중복 제거용
    
    for pattern in patterns:
        matches = re.findall(pattern, header_text, re.DOTALL)
        for name, params_str in matches:
            if name not in found_functions:
                param_list = parse_parameters(params_str)
                result.append((name, param_list))
                found_functions.add(name)
    
    return result

# 생성자 항상 새로 생성
def inject_constructor(header_text, class_name, functions_to_register):
    register_lines = [f'        REGISTER_BEHAVIOUR_MESSAGE({func});' for func in sorted(functions_to_register)]
    register_code = "\n".join(register_lines)

    # 기존 생성자에서 SetExecutionOrder() 호출 찾기
    execution_order_line = ""
    pattern_ctor = rf'\s*{class_name}\s*\(\s*\)\s*\{{([^}}]*)\}}'
    ctor_match = re.search(pattern_ctor, header_text, flags=re.DOTALL)
    
    if ctor_match:
        ctor_body = ctor_match.group(1)
        # SetExecutionOrder() 호출 찾기
        execution_order_match = re.search(r'SetExecutionOrder\s*\([^)]*\)\s*;', ctor_body)
        if execution_order_match:
            execution_order_line = execution_order_match.group(0).strip()
    
    # 기존 생성자 제거
    header_text = re.sub(pattern_ctor, '', header_text, flags=re.DOTALL)

    # 생성자 본문 구성
    constructor_body_lines = []
    if execution_order_line:
        constructor_body_lines.append(f"        {execution_order_line}")
    constructor_body_lines.extend(register_lines)
    
    constructor_body = "\n".join(constructor_body_lines)

    # public: 바로 뒤에 생성자 삽입
    constructor_code = (
        f"\n    {class_name}()\n"
        f"    {{\n"
        f"{constructor_body}\n"
        f"    }}"
    )
    pattern_public = r'(public\s*:)'
    new_text = re.sub(pattern_public, r'\1' + constructor_code, header_text, count=1)

    return new_text

# 헤더파일 처리
def process_header_file(header_path, signatures):
    with open(header_path, encoding="utf-8") as f:
        header_text = f.read()

    try:
        class_name = extract_class_name(header_text)
    except Exception as e:
        print(f"[{header_path.name}] 건너뜀: {e}")
        return

    declared_functions = extract_functions(header_text)

    functions_to_register = []
    for name, ptypes in declared_functions:
        if name in signatures:
            sig_types = signatures[name]
            # print(f"[DEBUG] {name}: 헤더={ptypes}, 시그니처={sig_types}")
            if sig_types == ptypes:
                functions_to_register.append(name)

    if not functions_to_register:
        print(f"[{header_path.name}] 등록할 함수 없음")
        return

    print(f"[{header_path.name}] 등록할 함수: {', '.join(functions_to_register)}")

    new_header_text = inject_constructor(header_text, class_name, functions_to_register)

    # 백업 생성
    backup_path = str(header_path) + ".bak"
    if os.path.exists(backup_path):
        os.remove(backup_path)
    os.rename(header_path, backup_path)

    with open(header_path, "w", encoding="utf-8") as f:
        f.write(new_header_text)

    print(f"[{header_path.name}] 완료 (백업: {backup_path})")

# 메인
def main():
    if len(sys.argv) != 3:
        print("사용법: python generate_registers.py <헤더폴더> <시그니처파일>")
        return

    headers_dir = Path(sys.argv[1])
    signatures_path = sys.argv[2]

    if not headers_dir.exists() or not headers_dir.is_dir():
        print("헤더폴더 경로가 유효하지 않습니다.")
        return

    signatures = load_signatures(signatures_path)
    
    # 디버깅용: 로드된 시그니처 출력
    # print("로드된 시그니처:")
    # for name, params in signatures.items():
    #     print(f"  {name}: {params}")
    # print()

    for header_file in headers_dir.glob("*.h"):
        process_header_file(header_file, signatures)

if __name__ == "__main__":
    main()
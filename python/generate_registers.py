import re
import sys
import os
from pathlib import Path

# 파라미터 문자열 정규화 함수 (전체 시그니처 유지)
def normalize_parameter(param_str):
    """파라미터 문자열을 정규화하되 전체 시그니처 유지"""
    if not param_str:
        return ""
    
    # 공백 정리
    param_str = param_str.strip()
    
    # 연속된 공백을 하나로 만들기
    param_str = re.sub(r'\s+', ' ', param_str)
    
    # 포인터와 참조 기호 주변 공백 정리
    param_str = re.sub(r'\s*\*\s*', '*', param_str)
    param_str = re.sub(r'\s*&\s*', '&', param_str)
    
    return param_str

# 파라미터 리스트 파싱 함수
def parse_parameters(params_str):
    """파라미터 문자열을 파싱하여 정규화된 파라미터 리스트 반환"""
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
                params.append(normalize_parameter(current_param))
            current_param = ""
            continue
        
        current_param += char
    
    # 마지막 파라미터 처리
    if current_param.strip():
        params.append(normalize_parameter(current_param))
    
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

# 매개변수 시그니처 비교 함수
def compare_signatures(sig_params, header_params):
    """시그니처 파일의 매개변수와 헤더의 매개변수를 정확히 비교"""
    if len(sig_params) != len(header_params):
        return False
    
    for sig_param, header_param in zip(sig_params, header_params):
        # 정확한 매칭 검사
        if sig_param != header_param:
            return False
    
    return True

# 생성자 항상 새로 생성
def inject_constructor(header_text, class_name, functions_to_register):
    # 기존 생성자를 찾아 SetExecutionOrder() 호출 등 REGISTER_BEHAVIOUR_MESSAGE 이외의 내용 추출
    existing_constructor_body_lines = []
    
    pattern_ctor = rf'\s*{class_name}\s*\(\s*\)\s*\{{([^}}]*)\}}'
    ctor_match = re.search(pattern_ctor, header_text, flags=re.DOTALL)
    
    if ctor_match:
        ctor_body = ctor_match.group(1)
        for line in ctor_body.splitlines():
            line_stripped = line.strip()
            # REGISTER_BEHAVIOUR_MESSAGE 라인은 건너뛰고, SetExecutionOrder 등 필요한 라인만 유지
            if line_stripped and not line_stripped.startswith("REGISTER_BEHAVIOUR_MESSAGE"):
                existing_constructor_body_lines.append(line_stripped)
    
    # 기존 생성자 제거 (새로운 내용을 삽입하기 위함)
    header_text = re.sub(pattern_ctor, '', header_text, flags=re.DOTALL)

    # REGISTER_BEHAVIOUR_MESSAGE 라인 구성
    register_lines = [f'        REGISTER_BEHAVIOUR_MESSAGE({func});' for func in sorted(functions_to_register)]

    # 생성자 본문 구성
    constructor_body_lines = []
    # 기존의 중요한 내용 (예: SetExecutionOrder) 먼저 추가
    for line in existing_constructor_body_lines:
        if line: # 빈 줄 제외
            constructor_body_lines.append(f"        {line}")
    
    # REGISTER_BEHAVIOUR_MESSAGE 라인 추가
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
    for name, header_params in declared_functions:
        if name in signatures:
            sig_params = signatures[name]
            
            # 디버깅 정보 출력
            # print(f"[DEBUG] {name}:")
            # print(f"  시그니처: {sig_params}")
            # print(f"  헤더:     {header_params}")
            
            # 정확한 시그니처 비교
            if compare_signatures(sig_params, header_params):
                functions_to_register.append(name)
                # print(f"  -> 매칭 성공!")
            # else:
            #     print(f"  -> 매칭 실패")

    # 등록할 함수가 없어도 생성자는 새로 생성하여 기존 REGISTER_BEHAVIOUR_MESSAGE 제거
    print(f"[{header_path.name}] 등록할 함수: {', '.join(functions_to_register) if functions_to_register else '없음'}")

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
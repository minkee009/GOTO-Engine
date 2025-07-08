import re
import sys
import os
from pathlib import Path

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
                params = match.group(2).strip()
                if not params:
                    param_list = []
                else:
                    # 공백 및 const 제거 (원하시면 더 고도화 가능)
                    param_list = [p.strip().replace("const ", "") for p in params.split(",")]
                signatures[name] = param_list
    return signatures

# 클래스 이름 추출
def extract_class_name(header_text):
    match = re.search(r'class\s+(\w+)\s*:\s*public\s+\w+', header_text)
    if match:
        return match.group(1)
    raise Exception("클래스 이름을 찾을 수 없습니다.")

# 헤더에서 함수 목록 추출
def extract_functions(header_text):
    pattern = r'void\s+(\w+)\s*\(([^)]*)\)'
    matches = re.findall(pattern, header_text)
    result = []
    for name, params in matches:
        params = params.strip()
        if not params:
            param_list = []
        else:
            param_list = [p.strip().replace("const ", "") for p in params.split(",")]
        result.append((name, param_list))
    return result

# 생성자 항상 새로 생성
def inject_constructor(header_text, class_name, functions_to_register):
    register_lines = [f'        REGISTER_BEHAVIOUR_MESSAGE({func});' for func in sorted(functions_to_register)]
    register_code = "\n".join(register_lines)

    # 기존 생성자 제거
    pattern_ctor = rf'\s*{class_name}\s*\(\s*\)\s*\{{[^}}]*\}}'
    header_text = re.sub(pattern_ctor, '', header_text, flags=re.DOTALL)

    # public: 바로 뒤에 생성자 삽입
    constructor_code = (
        f"\n    {class_name}()\n"
        f"    {{\n"
        f"{register_code}\n"
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
        if name in signatures and signatures[name] == ptypes:
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

    for header_file in headers_dir.glob("*.h"):
        process_header_file(header_file, signatures)

if __name__ == "__main__":
    main()
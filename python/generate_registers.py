import re
import sys
import os
from pathlib import Path

def load_signatures(signatures_path):
    signatures = set()
    with open(signatures_path, encoding="utf-8") as f:
        for line in f:
            line = line.strip()
            if not line:
                continue
            match = re.match(r'void\s+(\w+)\s*\(', line)
            if match:
                signatures.add(match.group(1))
    return signatures

def extract_class_name(header_text):
    match = re.search(r'class\s+(\w+)\s*:\s*public\s+\w+', header_text)
    if match:
        return match.group(1)
    raise Exception("클래스 이름을 찾을 수 없습니다.")

def extract_functions(header_text):
    pattern = r'void\s+(\w+)\s*\('
    return set(re.findall(pattern, header_text))

def has_default_constructor(header_text, class_name):
    pattern = rf'{class_name}\s*\(\s*\)'
    return re.search(pattern, header_text) is not None

def inject_constructor(header_text, class_name, functions_to_register):
    # 매크로 한 줄당 1탭 + 4칸 스페이스 들여쓰기(원하시면 조정 가능)
    indent = '    '  # 클래스 멤버 함수라 보통 4칸 스페이스
    macro_indent = indent * 2  # 생성자 내부이므로 2단계 들여쓰기

    register_lines = [f'{macro_indent}REGISTER_BEHAVIOUR_METHOD({func});' for func in sorted(functions_to_register)]
    register_code = '\n'.join(register_lines)

    # 기존 생성자 블럭 제거
    constructor_pattern = rf'{class_name}\s*\(\s*\)\s*\{{[^{{}}]*\}}'
    header_text_no_ctor = re.sub(constructor_pattern, '', header_text, flags=re.DOTALL)

    # 새 생성자 블럭 생성 (클래스 멤버 함수 스타일)
    new_constructor = f'\n{indent}{class_name}()\n{indent}{{\n{register_code}\n{indent}}}\n'

    # public: 뒤에 삽입
    public_pattern = r'(public\s*:)'

    if re.search(public_pattern, header_text_no_ctor):
        new_text = re.sub(public_pattern, r'\1' + new_constructor, header_text_no_ctor, count=1)
    else:
        new_text = new_constructor + header_text_no_ctor

    return new_text

def process_header_file(header_path, signatures):
    with open(header_path, encoding="utf-8") as f:
        header_text = f.read()

    try:
        class_name = extract_class_name(header_text)
    except Exception as e:
        print(f"[{header_path.name}] 건너뜀: {e}")
        return

    declared_functions = extract_functions(header_text)
    functions_to_register = signatures & declared_functions

    if not functions_to_register:
        print(f"[{header_path.name}] 등록할 함수 없음")
        return

    print(f"[{header_path.name}] 등록할 함수: {', '.join(functions_to_register)}")

    new_header_text = inject_constructor(header_text, class_name, functions_to_register)

    if new_header_text != header_text:
        backup_path = str(header_path) + ".bak"
        os.replace(header_path, backup_path)  # 항상 새로 백업
        with open(header_path, 'w', encoding="utf-8") as f:
            f.write(new_header_text)
        print(f"[{header_path.name}] 변경됨 -> 백업: {backup_path}")
    else:
        print(f"[{header_path.name}] 변경사항 없음")

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
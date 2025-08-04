import tkinter as tk
from tkinter import ttk, filedialog, messagebox
from PIL import Image, ImageTk
import os
import math

class GifToSpriteConverter:
    def __init__(self, root):
        self.root = root
        self.root.title("GIF to Sprite Sheet Converter")
        self.root.geometry("600x500")
        
        # 변수들
        self.gif_path = tk.StringVar()
        self.output_path = tk.StringVar()
        self.grid_mode = tk.StringVar(value="auto")  # auto 또는 manual
        self.manual_x = tk.StringVar(value="4")
        self.manual_y = tk.StringVar(value="4")
        self.output_format = tk.StringVar(value="PNG")
        
        self.gif_frames = []
        self.preview_image = None
        
        self.setup_ui()
    
    def setup_ui(self):
        main_frame = ttk.Frame(self.root, padding="10")
        main_frame.grid(row=0, column=0, sticky=(tk.W, tk.E, tk.N, tk.S))
        
        # GIF 파일 선택
        ttk.Label(main_frame, text="GIF 파일:").grid(row=0, column=0, sticky=tk.W, pady=5)
        
        file_frame = ttk.Frame(main_frame)
        file_frame.grid(row=1, column=0, columnspan=2, sticky=(tk.W, tk.E), pady=5)
        
        ttk.Entry(file_frame, textvariable=self.gif_path, width=50).grid(row=0, column=0, sticky=(tk.W, tk.E))
        ttk.Button(file_frame, text="찾기", command=self.browse_gif).grid(row=0, column=1, padx=(5, 0))
        
        file_frame.columnconfigure(0, weight=1)
        
        # 그리드 설정
        ttk.Label(main_frame, text="그리드 설정:").grid(row=2, column=0, sticky=tk.W, pady=(20, 5))
        
        grid_frame = ttk.Frame(main_frame)
        grid_frame.grid(row=3, column=0, columnspan=2, sticky=(tk.W, tk.E), pady=5)
        
        # 자동 모드
        ttk.Radiobutton(grid_frame, text="자동 (프레임 수 기반)", 
                       variable=self.grid_mode, value="auto", 
                       command=self.update_grid_mode).grid(row=0, column=0, sticky=tk.W)
        
        # 수동 모드
        ttk.Radiobutton(grid_frame, text="수동:", 
                       variable=self.grid_mode, value="manual",
                       command=self.update_grid_mode).grid(row=1, column=0, sticky=tk.W)
        
        self.manual_frame = ttk.Frame(grid_frame)
        self.manual_frame.grid(row=1, column=1, sticky=tk.W, padx=(10, 0))
        
        ttk.Label(self.manual_frame, text="X:").grid(row=0, column=0)
        self.x_entry = ttk.Entry(self.manual_frame, textvariable=self.manual_x, width=5)
        self.x_entry.grid(row=0, column=1, padx=5)
        
        ttk.Label(self.manual_frame, text="Y:").grid(row=0, column=2)
        self.y_entry = ttk.Entry(self.manual_frame, textvariable=self.manual_y, width=5)
        self.y_entry.grid(row=0, column=3, padx=5)
        
        # 출력 형식
        ttk.Label(main_frame, text="출력 형식:").grid(row=4, column=0, sticky=tk.W, pady=(20, 5))
        
        format_frame = ttk.Frame(main_frame)
        format_frame.grid(row=5, column=0, columnspan=2, sticky=(tk.W, tk.E), pady=5)
        
        ttk.Radiobutton(format_frame, text="PNG (무압축)", 
                       variable=self.output_format, value="PNG").grid(row=0, column=0, sticky=tk.W)
        ttk.Radiobutton(format_frame, text="JPEG", 
                       variable=self.output_format, value="JPEG").grid(row=0, column=1, sticky=tk.W, padx=(20, 0))
        
        # 출력 경로
        ttk.Label(main_frame, text="저장 경로:").grid(row=6, column=0, sticky=tk.W, pady=(20, 5))
        
        output_frame = ttk.Frame(main_frame)
        output_frame.grid(row=7, column=0, columnspan=2, sticky=(tk.W, tk.E), pady=5)
        
        ttk.Entry(output_frame, textvariable=self.output_path, width=50).grid(row=0, column=0, sticky=(tk.W, tk.E))
        ttk.Button(output_frame, text="찾기", command=self.browse_output).grid(row=0, column=1, padx=(5, 0))
        
        output_frame.columnconfigure(0, weight=1)
        
        # 정보 표시
        self.info_label = ttk.Label(main_frame, text="GIF 파일을 선택하세요")
        self.info_label.grid(row=8, column=0, columnspan=2, pady=20)
        
        # 변환 버튼
        ttk.Button(main_frame, text="스프라이트 시트 생성", 
                  command=self.convert_to_sprite).grid(row=9, column=0, columnspan=2, pady=20)
        
        # 프리뷰 (작은 크기)
        self.preview_label = ttk.Label(main_frame, text="프리뷰")
        self.preview_label.grid(row=10, column=0, columnspan=2, pady=10)
        
        main_frame.columnconfigure(0, weight=1)
        self.root.columnconfigure(0, weight=1)
        self.root.rowconfigure(0, weight=1)
        
        self.update_grid_mode()
    
    def update_grid_mode(self):
        if self.grid_mode.get() == "manual":
            for widget in self.manual_frame.winfo_children():
                widget.configure(state="normal")
        else:
            for widget in self.manual_frame.winfo_children():
                if isinstance(widget, ttk.Entry):
                    widget.configure(state="disabled")
    
    def browse_gif(self):
        filename = filedialog.askopenfilename(
            title="GIF 파일 선택",
            filetypes=[("GIF files", "*.gif"), ("All files", "*.*")]
        )
        if filename:
            self.gif_path.set(filename)
            self.load_gif_info()
    
    def browse_output(self):
        filename = filedialog.asksaveasfilename(
            title="저장할 파일명",
            defaultextension=f".{self.output_format.get().lower()}",
            filetypes=[
                ("PNG files", "*.png"),
                ("JPEG files", "*.jpg"),
                ("All files", "*.*")
            ]
        )
        if filename:
            self.output_path.set(filename)
    
    def load_gif_info(self):
        try:
            gif = Image.open(self.gif_path.get())
            
            # 프레임 수 계산
            frame_count = 0
            self.gif_frames = []
            
            try:
                while True:
                    # 프레임을 RGBA로 변환하여 저장
                    frame = gif.copy().convert('RGBA')
                    self.gif_frames.append(frame)
                    frame_count += 1
                    gif.seek(frame_count)
            except EOFError:
                pass
            
            # 자동 그리드 계산 (가능한 한 정사각형에 가깝게)
            auto_x = math.ceil(math.sqrt(frame_count))
            auto_y = math.ceil(frame_count / auto_x)
            
            # 정보 업데이트
            frame_size = self.gif_frames[0].size
            info_text = f"프레임 수: {frame_count}, 크기: {frame_size[0]}x{frame_size[1]}, 권장 그리드: {auto_x}x{auto_y}"
            self.info_label.config(text=info_text)
            
            # 기본 출력 경로 설정
            if not self.output_path.get():
                base_name = os.path.splitext(self.gif_path.get())[0]
                ext = "png" if self.output_format.get() == "PNG" else "jpg"
                self.output_path.set(f"{base_name}_spritesheet.{ext}")
            
            # 첫 번째 프레임 프리뷰 생성
            self.create_preview()
            
        except Exception as e:
            messagebox.showerror("오류", f"GIF 파일을 읽을 수 없습니다: {str(e)}")
    
    def create_preview(self):
        if not self.gif_frames:
            return
        
        try:
            # 작은 프리뷰 생성 (첫 번째 프레임만)
            preview_frame = self.gif_frames[0].copy()
            preview_frame.thumbnail((100, 100), Image.Resampling.LANCZOS)
            
            self.preview_image = ImageTk.PhotoImage(preview_frame)
            self.preview_label.config(image=self.preview_image, text="")
            
        except Exception as e:
            self.preview_label.config(text=f"프리뷰 오류: {str(e)}")
    
    def convert_to_sprite(self):
        if not self.gif_frames:
            messagebox.showerror("오류", "GIF 파일을 먼저 선택하세요")
            return
        
        if not self.output_path.get():
            messagebox.showerror("오류", "출력 경로를 설정하세요")
            return
        
        try:
            frame_count = len(self.gif_frames)
            frame_size = self.gif_frames[0].size
            
            # 그리드 크기 결정
            if self.grid_mode.get() == "auto":
                grid_x = math.ceil(math.sqrt(frame_count))
                grid_y = math.ceil(frame_count / grid_x)
            else:
                grid_x = int(self.manual_x.get())
                grid_y = int(self.manual_y.get())
                
                if grid_x * grid_y < frame_count:
                    if not messagebox.askyesno("경고", 
                        f"그리드 크기({grid_x}x{grid_y}={grid_x*grid_y})가 프레임 수({frame_count})보다 작습니다. "
                        "일부 프레임이 누락될 수 있습니다. 계속하시겠습니까?"):
                        return
            
            # 스프라이트 시트 생성
            sprite_width = frame_size[0] * grid_x
            sprite_height = frame_size[1] * grid_y
            
            # 투명 배경으로 스프라이트 시트 생성
            sprite_sheet = Image.new('RGBA', (sprite_width, sprite_height), (0, 0, 0, 0))
            
            # 프레임 배치
            for i, frame in enumerate(self.gif_frames):
                if i >= grid_x * grid_y:
                    break
                
                x = (i % grid_x) * frame_size[0]
                y = (i // grid_x) * frame_size[1]
                
                sprite_sheet.paste(frame, (x, y))
            
            # 저장
            if self.output_format.get() == "PNG":
                sprite_sheet.save(self.output_path.get(), "PNG", optimize=False, compress_level=0)
            else:
                # JPEG는 투명도를 지원하지 않으므로 흰 배경으로 변환
                rgb_sprite = Image.new('RGB', sprite_sheet.size, (255, 255, 255))
                rgb_sprite.paste(sprite_sheet, mask=sprite_sheet.split()[-1])
                rgb_sprite.save(self.output_path.get(), "JPEG", quality=100)
            
            messagebox.showinfo("완료", 
                f"스프라이트 시트가 생성되었습니다!\n"
                f"크기: {sprite_width}x{sprite_height}\n"
                f"그리드: {grid_x}x{grid_y}\n"
                f"저장됨: {self.output_path.get()}")
            
        except Exception as e:
            messagebox.showerror("오류", f"변환 중 오류가 발생했습니다: {str(e)}")

def main():
    root = tk.Tk()
    app = GifToSpriteConverter(root)
    root.mainloop()

if __name__ == "__main__":
    main()
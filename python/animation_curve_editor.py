import tkinter as tk
from tkinter import ttk, messagebox, filedialog
import math
import json

class HermitCurve:
    def __init__(self):
        self.keyframes = [] # Each keyframe: [time, value, in_tangent, out_tangent, tangent_mode]

    def add_keyframe(self, time, value, in_tan=0.0, out_tan=0.0, tangent_mode=1):
        self.keyframes.append([time, value, in_tan, out_tan, tangent_mode])
        self.keyframes.sort(key=lambda kf: kf[0])

    def to_json(self):
        # 키프레임 리스트를 딕셔너리 리스트로 변환
        json_keyframes = []
        for kf in self.keyframes:
            json_keyframes.append({
                "time": kf[0],
                "value": kf[1],
                "in_tangent": kf[2],
                "out_tangent": kf[3],
                "tangent_mode": kf[4]
            })
        return {"keyframes": json_keyframes}
    
    def remove_keyframe(self, index):
        if 0 <= index < len(self.keyframes):
            self.keyframes.pop(index)
    
    def evaluate(self, time):
        if not self.keyframes:
            return 0.0
        
        if len(self.keyframes) == 1:
            return self.keyframes[0][1]
        
        # Find the segment
        if time <= self.keyframes[0][0]:
            return self.keyframes[0][1]
        if time >= self.keyframes[-1][0]:
            return self.keyframes[-1][1]
        
        for i in range(len(self.keyframes) - 1):
            t0, v0, _, out_tan, _ = self.keyframes[i]
            t1, v1, in_tan, _, _ = self.keyframes[i + 1]
            
            if t0 <= time <= t1:
                # Hermite interpolation
                dt = t1 - t0
                if dt == 0:
                    return v0
                
                t = (time - t0) / dt
                t2 = t * t
                t3 = t2 * t
                
                # Hermite basis functions
                h00 = 2 * t3 - 3 * t2 + 1
                h10 = t3 - 2 * t2 + t
                h01 = -2 * t3 + 3 * t2
                h11 = t3 - t2
                
                # Scale tangents by time delta
                m0 = out_tan * dt
                m1 = in_tan * dt
                
                return h00 * v0 + h10 * m0 + h01 * v1 + h11 * m1
        
        return 0.0

class KeyframeEditDialog:
    def __init__(self, parent, keyframe_data):
        self.result = None
        self.dialog = tk.Toplevel(parent)
        self.dialog.title('Edit Keyframe')
        self.dialog.geometry('300x250')
        self.dialog.configure(bg='#2b2b2b')
        self.dialog.transient(parent)
        # self.dialog.grab_set() # 이 부분을 제거합니다.

        # Center the dialog
        self.dialog.update_idletasks()
        x = (self.dialog.winfo_screenwidth() // 2) - (300 // 2)
        y = (self.dialog.winfo_screenheight() // 2) - (250 // 2)
        self.dialog.geometry(f'300x250+{x}+{y}')
        
        # Extract current values
        time, value, in_tan, out_tan, mode = keyframe_data
        
        # Create form
        frame = tk.Frame(self.dialog, bg='#2b2b2b')
        frame.pack(fill=tk.BOTH, expand=True, padx=20, pady=20)
        
        # Time
        tk.Label(frame, text='Time:', bg='#2b2b2b', fg='white').grid(row=0, column=0, sticky='w', pady=5)
        self.time_var = tk.StringVar(value=str(time))
        time_entry = tk.Entry(frame, textvariable=self.time_var, width=15)
        time_entry.grid(row=0, column=1, pady=5, padx=(10, 0))
        
        # Value
        tk.Label(frame, text='Value:', bg='#2b2b2b', fg='white').grid(row=1, column=0, sticky='w', pady=5)
        self.value_var = tk.StringVar(value=str(value))
        value_entry = tk.Entry(frame, textvariable=self.value_var, width=15)
        value_entry.grid(row=1, column=1, pady=5, padx=(10, 0))
        
        # In Tangent
        tk.Label(frame, text='In Tangent:', bg='#2b2b2b', fg='white').grid(row=2, column=0, sticky='w', pady=5)
        self.in_tan_var = tk.StringVar(value=str(in_tan))
        in_tan_entry = tk.Entry(frame, textvariable=self.in_tan_var, width=15)
        in_tan_entry.grid(row=2, column=1, pady=5, padx=(10, 0))
        
        # Out Tangent
        tk.Label(frame, text='Out Tangent:', bg='#2b2b2b', fg='white').grid(row=3, column=0, sticky='w', pady=5)
        self.out_tan_var = tk.StringVar(value=str(out_tan))
        out_tan_entry = tk.Entry(frame, textvariable=self.out_tan_var, width=15)
        out_tan_entry.grid(row=3, column=1, pady=5, padx=(10, 0))
        
        # Buttons
        button_frame = tk.Frame(frame, bg='#2b2b2b')
        button_frame.grid(row=4, column=0, columnspan=2, pady=20)
        
        ok_button = tk.Button(button_frame, text='OK', command=self.ok_clicked, width=10)
        ok_button.pack(side=tk.LEFT, padx=5)
        
        cancel_button = tk.Button(button_frame, text='Cancel', command=self.cancel_clicked, width=10)
        cancel_button.pack(side=tk.LEFT, padx=5)
        
        # Focus on first entry
        time_entry.focus_set()
        time_entry.select_range(0, tk.END)
        
        # Bind Enter and Escape
        self.dialog.bind('<Return>', lambda e: self.ok_clicked())
        self.dialog.bind('<Escape>', lambda e: self.cancel_clicked())

    def ok_clicked(self):
        try:
            self.result = [
                float(self.time_var.get()),
                float(self.value_var.get()),
                float(self.in_tan_var.get()),
                float(self.out_tan_var.get()),
                1  # Keep current mode (will be preserved by caller)
            ]
            self.dialog.destroy()
        except ValueError:
            messagebox.showerror('Error', 'Please enter valid numbers')
    
    def cancel_clicked(self):
        self.result = None
        self.dialog.destroy()

class CurveEditor:
    def __init__(self, parent):
        self.parent = parent
        self.app = None  # Will be set by the main app
        self.canvas = tk.Canvas(parent, bg='#2b2b2b', width=400, height=300)
        self.canvas.pack(fill=tk.BOTH, expand=True)
        
        self.curve = HermitCurve()
        self.view_offset_x = 0
        self.view_offset_y = 0
        self.zoom = 50  # pixels per unit
        self.selected_keyframe = -1
        self.dragging_keyframe = False
        self.dragging_tangent = None  # 'in' or 'out'
        self.dragging_view = False
        self.last_mouse_x = 0
        self.last_mouse_y = 0
        
        # Bind events
        self.canvas.bind('<Button-1>', self.on_click)
        self.canvas.bind('<Button-2>', self.on_middle_click)
        self.canvas.bind('<Button-3>', self.on_right_click)
        self.canvas.bind('<Double-Button-1>', self.on_double_click)
        self.canvas.bind('<B1-Motion>', self.on_drag)
        self.canvas.bind('<B2-Motion>', self.on_middle_drag)
        self.canvas.bind('<ButtonRelease-1>', self.on_release)
        self.canvas.bind('<ButtonRelease-2>', self.on_middle_release)
        self.canvas.bind('<Motion>', self.on_mouse_move)
        self.canvas.bind('<MouseWheel>', self.on_wheel)
        self.canvas.bind('<KeyPress>', self.on_key)
        self.canvas.bind('<KeyRelease>', lambda e: None)  # Prevent key repeat issues
        self.canvas.focus_set()
        
        # Make canvas focusable and keep focus
        self.canvas.bind('<FocusOut>', lambda e: self.canvas.focus_set())
        self.canvas.bind('<Button>', lambda e: self.canvas.focus_set())
        
        # Add some default keyframes
        self.curve.add_keyframe(0, 0, 0, 1, 1)
        self.curve.add_keyframe(1, 1, 1, 0, 1)
        
        self.redraw()
    
    def screen_to_curve(self, x, y):
        curve_x = (x - self.canvas.winfo_width() / 2 - self.view_offset_x) / self.zoom
        curve_y = -(y - self.canvas.winfo_height() / 2 - self.view_offset_y) / self.zoom
        return curve_x, curve_y
    
    def curve_to_screen(self, x, y):
        screen_x = x * self.zoom + self.canvas.winfo_width() / 2 + self.view_offset_x
        screen_y = -y * self.zoom + self.canvas.winfo_height() / 2 + self.view_offset_y
        return screen_x, screen_y
    
    def find_keyframe_at(self, x, y, tolerance=10):
        for i, (t, v, _, _, _) in enumerate(self.curve.keyframes):
            sx, sy = self.curve_to_screen(t, v)
            if abs(x - sx) <= tolerance and abs(y - sy) <= tolerance:
                return i
        return -1
    
    def find_tangent_handle_at(self, x, y, tolerance=8):
        if self.selected_keyframe >= 0:
            kf = self.curve.keyframes[self.selected_keyframe]
            t, v, in_tan, out_tan, _ = kf
            sx, sy = self.curve_to_screen(t, v)
            
            # In tangent handle
            in_x = sx - 40
            in_y = sy - in_tan * 40
            if abs(x - in_x) <= tolerance and abs(y - in_y) <= tolerance:
                return 'in'
            
            # Out tangent handle
            out_x = sx + 40
            out_y = sy - out_tan * 40
            if abs(x - out_x) <= tolerance and abs(y - out_y) <= tolerance:
                return 'out'
        
        return None
    
    def edit_keyframe_fallback(self):
        """Fallback method using simple input dialogs"""
        if self.selected_keyframe >= 0:
            kf = self.curve.keyframes[self.selected_keyframe]
            
            try:
                from tkinter import simpledialog
                
                # Get new values using simple dialogs
                new_time = simpledialog.askfloat("Edit Keyframe", f"Time (current: {kf[0]:.3f}):", 
                                                initialvalue=kf[0])
                if new_time is None:
                    return
                
                new_value = simpledialog.askfloat("Edit Keyframe", f"Value (current: {kf[1]:.3f}):", 
                                                 initialvalue=kf[1])
                if new_value is None:
                    return
                
                new_in_tan = simpledialog.askfloat("Edit Keyframe", f"In Tangent (current: {kf[2]:.3f}):", 
                                                  initialvalue=kf[2])
                if new_in_tan is None:
                    return
                
                new_out_tan = simpledialog.askfloat("Edit Keyframe", f"Out Tangent (current: {kf[3]:.3f}):", 
                                                   initialvalue=kf[3])
                if new_out_tan is None:
                    return
                
                # Update keyframe
                self.curve.keyframes[self.selected_keyframe] = [new_time, new_value, new_in_tan, new_out_tan, kf[4]]
                
                # Re-sort keyframes by time
                self.curve.keyframes.sort(key=lambda kf: kf[0])
                
                # Find new index of edited keyframe
                for i, (t, _, _, _, _) in enumerate(self.curve.keyframes):
                    if abs(t - new_time) < 0.001:
                        self.selected_keyframe = i
                        break
                
                self.redraw()
                self.app.update_preview()  
                
            except Exception as e:
                print(f"Error in fallback edit: {e}")
                messagebox.showerror("Error", "Failed to edit keyframe")
    
    def update_unified_tangent(self, keyframe_index, tangent_type, new_value):
        """Update tangent with Unity-style unified behavior (parallel, not mirrored)"""
        if 0 <= keyframe_index < len(self.curve.keyframes):
            kf = self.curve.keyframes[keyframe_index]
            tangent_mode = kf[4]
            
            if tangent_mode == 1:  # Unified mode - parallel tangents
                if tangent_type == 'in':
                    self.curve.keyframes[keyframe_index][2] = new_value
                    self.curve.keyframes[keyframe_index][3] = new_value  # Same direction (parallel)
                elif tangent_type == 'out':
                    self.curve.keyframes[keyframe_index][3] = new_value
                    self.curve.keyframes[keyframe_index][2] = new_value  # Same direction (parallel)
            else:  # Broken mode - independent
                if tangent_type == 'in':
                    self.curve.keyframes[keyframe_index][2] = new_value
                elif tangent_type == 'out':
                    self.curve.keyframes[keyframe_index][3] = new_value
    
    def toggle_tangent_mode(self):
        """Toggle between unified and broken tangent mode for selected keyframe"""
        if self.selected_keyframe >= 0:
            current_mode = self.curve.keyframes[self.selected_keyframe][4]
            new_mode = 0 if current_mode == 1 else 1  # Toggle between 0 (broken) and 1 (unified)
            self.curve.keyframes[self.selected_keyframe][4] = new_mode
            self.redraw()
    
    def edit_selected_keyframe(self):
        """Open edit dialog for selected keyframe"""
        if self.selected_keyframe >= 0:
            kf = self.curve.keyframes[self.selected_keyframe]
            try:
                # Use root window as parent
                root_window = self.canvas.winfo_toplevel()
                dialog = KeyframeEditDialog(root_window, kf)
                
                # dialog.dialog.grab_set() 대신 wait_window를 사용합니다.
                root_window.wait_window(dialog.dialog) # 다이얼로그가 닫힐 때까지 기다립니다.

                if dialog.result:
                    dialog.result[4] = kf[4]  # preserve tangent mode
                    self.curve.keyframes[self.selected_keyframe] = dialog.result
                    self.curve.keyframes.sort(key=lambda kf: kf[0])
                    # Re-find index after sort, as the time might have changed
                    new_time_val = dialog.result[0]
                    for i, (t, _, _, _, _) in enumerate(self.curve.keyframes):
                        if abs(t - new_time_val) < 0.001:
                            self.selected_keyframe = i
                            break
                    self.redraw()
                    self.app.update_preview()  

            except Exception as e:
                print(f"Error opening edit dialog: {e}")
                self.edit_keyframe_fallback()
    
    def update_preview(self):
        """Safe method to update preview"""
        if hasattr(self, 'app') and self.app:
            self.app.update_preview()
    
    def on_click(self, event):
        # Ensure canvas has focus for keyboard events
        self.canvas.focus_set()
        
        self.last_mouse_x = event.x
        self.last_mouse_y = event.y
        
        # Check for tangent handles first
        tangent = self.find_tangent_handle_at(event.x, event.y)
        if tangent:
            self.dragging_tangent = tangent
            return
        
        # Check for keyframes
        kf_index = self.find_keyframe_at(event.x, event.y)
        if kf_index >= 0:
            self.selected_keyframe = kf_index
            self.dragging_keyframe = True
        else:
            # Add new keyframe
            curve_x, curve_y = self.screen_to_curve(event.x, event.y)
            self.curve.add_keyframe(curve_x, curve_y, 0, 0, 1)
            self.selected_keyframe = -1
            for i, (t, _, _, _, _) in enumerate(self.curve.keyframes):
                if abs(t - curve_x) < 0.01:
                    self.selected_keyframe = i
                    break
        
        self.redraw()
        self.update_preview()
    
    def on_double_click(self, event):
        # Double-click to edit keyframe
        kf_index = self.find_keyframe_at(event.x, event.y)
        if kf_index >= 0:
            self.selected_keyframe = kf_index
            self.edit_selected_keyframe()
    
    def on_middle_click(self, event):
        self.dragging_view = True
        self.last_mouse_x = event.x
        self.last_mouse_y = event.y
    
    def on_right_click(self, event):
        # Check if right-clicking on selected keyframe for context menu
        kf_index = self.find_keyframe_at(event.x, event.y)
        if kf_index >= 0 and kf_index == self.selected_keyframe:
            # Toggle tangent mode
            self.toggle_tangent_mode()
        elif kf_index >= 0 and len(self.curve.keyframes) > 1:
            # Delete keyframe
            self.curve.remove_keyframe(kf_index)
            if self.selected_keyframe >= kf_index:
                self.selected_keyframe = max(-1, self.selected_keyframe - 1)
            self.redraw()
            self.update_preview()
    
    def on_drag(self, event):
        if self.dragging_tangent and self.selected_keyframe >= 0:
            kf = self.curve.keyframes[self.selected_keyframe]
            t, v = kf[0], kf[1]
            sx, sy = self.curve_to_screen(t, v)
            
            if self.dragging_tangent == 'in':
                tangent = -(event.y - sy) / 40
                self.update_unified_tangent(self.selected_keyframe, 'in', tangent)
            elif self.dragging_tangent == 'out':
                tangent = -(event.y - sy) / 40
                self.update_unified_tangent(self.selected_keyframe, 'out', tangent)
            
        elif self.dragging_keyframe and self.selected_keyframe >= 0:
            curve_x, curve_y = self.screen_to_curve(event.x, event.y)
            self.curve.keyframes[self.selected_keyframe][0] = curve_x
            self.curve.keyframes[self.selected_keyframe][1] = curve_y
            # Re-sort keyframes by time
            self.curve.keyframes.sort(key=lambda kf: kf[0])
            # Find new index of selected keyframe
            for i, (t, _, _, _, _) in enumerate(self.curve.keyframes):
                if abs(t - curve_x) < 0.01:
                    self.selected_keyframe = i
                    break
        
        self.redraw()
        self.app.update_preview()  
    
    def on_middle_drag(self, event):
        if self.dragging_view:
            dx = event.x - self.last_mouse_x
            dy = event.y - self.last_mouse_y
            self.view_offset_x += dx
            self.view_offset_y += dy
            self.last_mouse_x = event.x
            self.last_mouse_y = event.y
            self.redraw()
    
    def on_release(self, event):
        self.dragging_keyframe = False
        self.dragging_tangent = None
    
    def on_middle_release(self, event):
        self.dragging_view = False
    
    def on_mouse_move(self, event):
        pass
    
    def on_wheel(self, event):
        # Zoom
        old_zoom = self.zoom
        if event.delta > 0:
            self.zoom *= 1.1
        else:
            self.zoom /= 1.1
        self.zoom = max(10, min(200, self.zoom))
        
        # Adjust view offset to zoom towards mouse
        mouse_curve_x, mouse_curve_y = self.screen_to_curve(event.x, event.y)
        new_mouse_x, new_mouse_y = self.curve_to_screen(mouse_curve_x, mouse_curve_y)
        self.view_offset_x += event.x - new_mouse_x
        self.view_offset_y += event.y - new_mouse_y
        
        self.redraw()
    
    def on_key(self, event):
        print(f"Key pressed: {event.keysym}")  # Debug print
        
        if event.keysym == 'Delete' and self.selected_keyframe >= 0:
            if len(self.curve.keyframes) > 1:
                self.curve.remove_keyframe(self.selected_keyframe)
                self.selected_keyframe = -1
                self.redraw()
                self.app.update_preview()  
                print("Keyframe deleted")  # Debug print
        elif event.keysym in ['t', 'T'] and self.selected_keyframe >= 0:
            # Toggle tangent mode with 't' key
            self.toggle_tangent_mode()
            print("Tangent mode toggled")  # Debug print
        elif event.keysym in ['e', 'E'] and self.selected_keyframe >= 0:
            # Edit keyframe with 'e' key
            self.edit_selected_keyframe()
            print("Edit keyframe")  # Debug print
    
    def redraw(self):
        self.canvas.delete('all')
        width = self.canvas.winfo_width()
        height = self.canvas.winfo_height()
        
        # Draw grid
        center_x = width / 2 + self.view_offset_x
        center_y = height / 2 + self.view_offset_y
        
        # Vertical lines
        for i in range(-20, 21):
            x = center_x + i * self.zoom
            if 0 <= x <= width:
                color = '#444' if i == 0 else '#333'
                self.canvas.create_line(x, 0, x, height, fill=color)
        
        # Horizontal lines
        for i in range(-20, 21):
            y = center_y - i * self.zoom
            if 0 <= y <= height:
                color = '#444' if i == 0 else '#333'
                self.canvas.create_line(0, y, width, y, fill=color)
        
        # Draw curve
        if len(self.curve.keyframes) >= 2:
            points = []
            min_time = self.curve.keyframes[0][0]
            max_time = self.curve.keyframes[-1][0]
            
            for i in range(200):
                t = min_time + (max_time - min_time) * i / 199
                value = self.curve.evaluate(t)
                x, y = self.curve_to_screen(t, value)
                # 필터링하여 유효한 좌표만 추가
                if 0 <= x <= width and 0 <= y <= height: # 이 조건 제거
                     points.extend([x, y])
            
            # 모든 점을 그릴 수 있도록 필터링 조건 변경
            if len(points) >= 4:
                self.canvas.create_line(points, fill='#4CAF50', width=2, smooth=True)
        
        # Draw keyframes
        for i, (t, v, in_tan, out_tan, tangent_mode) in enumerate(self.curve.keyframes):
            x, y = self.curve_to_screen(t, v)
            if 0 <= x <= width and 0 <= y <= height:
                # Different colors based on tangent mode
                if i == self.selected_keyframe:
                    if tangent_mode == 1:  # Unified
                        color = '#FFA726'
                        outline = 'white'
                    else:  # Broken
                        color = '#FF5722'
                        outline = 'yellow'
                else:
                    color = '#FFD54F'
                    outline = 'white'
                
                self.canvas.create_oval(x-6, y-6, x+6, y+6, fill=color, outline=outline, width=2)
                
                # Draw tangent handles for selected keyframe
                if i == self.selected_keyframe:
                    # In tangent
                    in_x = x - 40
                    in_y = y - in_tan * 40
                    line_color = '#FF5722' if tangent_mode == 1 else '#9E9E9E'
                    self.canvas.create_line(x, y, in_x, in_y, fill=line_color, width=2)
                    self.canvas.create_oval(in_x-4, in_y-4, in_x+4, in_y+4, fill=line_color, outline='white')
                    
                    # Out tangent
                    out_x = x + 40
                    out_y = y - out_tan * 40
                    self.canvas.create_line(x, y, out_x, out_y, fill=line_color, width=2)
                    self.canvas.create_oval(out_x-4, out_y-4, out_x+4, out_y+4, fill=line_color, outline='white')
        
        # Show tangent mode and keyframe info
        if self.selected_keyframe >= 0:
            kf = self.curve.keyframes[self.selected_keyframe]
            mode_text = "Unified" if kf[4] == 1 else "Broken"
            info_text = f'Keyframe {self.selected_keyframe}: Time={kf[0]:.3f}, Value={kf[1]:.3f} | Mode: {mode_text}'
            self.canvas.create_text(10, height-35, anchor='sw', 
                                   text=info_text, 
                                   fill='white', font=('Arial', 9))
            self.canvas.create_text(10, height-20, anchor='sw', 
                                   text='Double-click or E: Edit | Right-click or T: Toggle mode', 
                                   fill='#aaa', font=('Arial', 8))

class PreviewPanel:
    def __init__(self, parent, curve_editor):
        self.parent = parent
        self.curve_editor = curve_editor
        
        # Create single canvas (no tabs)
        self.canvas = tk.Canvas(parent, bg='#1e1e1e', width=400, height=300)
        self.canvas.pack(fill=tk.BOTH, expand=True)
        
        self.time = 0.0
        self.animation_speed = 1.0
        self.start_time = 0.0
        self.end_time = 2.0
        
        # Transform settings
        self.transform_mode = tk.StringVar(value='translation')  # translation, rotation, scale
        self.axis_mode = tk.StringVar(value='both')  # x, y, both
        
        # Create control frame
        control_frame = tk.Frame(parent, bg='#2b2b2b')
        control_frame.pack(fill=tk.X, padx=5, pady=5)
        
        # Time range controls
        time_range_frame = tk.Frame(control_frame, bg='#2b2b2b')
        time_range_frame.pack(fill=tk.X, pady=2)
        
        tk.Label(time_range_frame, text='Start:', bg='#2b2b2b', fg='white').pack(side=tk.LEFT)
        self.start_time_var = tk.DoubleVar(value=0.0)
        start_spinner = tk.Spinbox(time_range_frame, from_=-10, to=10, increment=0.1, 
                                  textvariable=self.start_time_var, width=8,
                                  command=self.on_time_range_change)
        start_spinner.pack(side=tk.LEFT, padx=2)
        
        tk.Label(time_range_frame, text='End:', bg='#2b2b2b', fg='white').pack(side=tk.LEFT, padx=(10,0))
        self.end_time_var = tk.DoubleVar(value=2.0)
        end_spinner = tk.Spinbox(time_range_frame, from_=-10, to=10, increment=0.1,
                                textvariable=self.end_time_var, width=8,
                                command=self.on_time_range_change)
        end_spinner.pack(side=tk.LEFT, padx=2)
        
        # Time slider
        time_slider_frame = tk.Frame(control_frame, bg='#2b2b2b')
        time_slider_frame.pack(fill=tk.X, pady=2)
        
        tk.Label(time_slider_frame, text='Time:', bg='#2b2b2b', fg='white').pack(side=tk.LEFT)
        self.time_var = tk.DoubleVar(value=0)
        self.time_slider = tk.Scale(time_slider_frame, from_=self.start_time, to=self.end_time, 
                                   resolution=0.01, orient=tk.HORIZONTAL, variable=self.time_var,
                                   command=self.on_time_change, bg='#2b2b2b', fg='white')
        self.time_slider.pack(side=tk.LEFT, fill=tk.X, expand=True)
        
        # Animation controls
        control_frame2 = tk.Frame(control_frame, bg='#2b2b2b')
        control_frame2.pack(fill=tk.X, padx=5, pady=2)
        
        self.play_button = tk.Button(control_frame2, text='Play', command=self.toggle_animation)
        self.play_button.pack(side=tk.LEFT, padx=2)
        
        tk.Label(control_frame2, text='Speed:', bg='#2b2b2b', fg='white').pack(side=tk.LEFT, padx=(10,0))
        self.speed_var = tk.DoubleVar(value=1.0)
        speed_slider = tk.Scale(control_frame2, from_=0.1, to=3.0, resolution=0.1,
                               orient=tk.HORIZONTAL, variable=self.speed_var,
                               bg='#2b2b2b', fg='white', length=100)
        speed_slider.pack(side=tk.LEFT)
        
        # Transform selection (radio buttons)
        transform_frame = tk.Frame(control_frame, bg='#2b2b2b')
        transform_frame.pack(fill=tk.X, padx=5, pady=2)
        
        tk.Label(transform_frame, text='Transform:', bg='#2b2b2b', fg='white').pack(side=tk.LEFT)
        
        tk.Radiobutton(transform_frame, text='Translation', variable=self.transform_mode, value='translation',
                      command=self.update_display, bg='#2b2b2b', fg='white',
                      selectcolor='#2b2b2b').pack(side=tk.LEFT, padx=5)
        tk.Radiobutton(transform_frame, text='Rotation', variable=self.transform_mode, value='rotation',
                      command=self.update_display, bg='#2b2b2b', fg='white',
                      selectcolor='#2b2b2b').pack(side=tk.LEFT, padx=5)
        tk.Radiobutton(transform_frame, text='Scale', variable=self.transform_mode, value='scale',
                      command=self.update_display, bg='#2b2b2b', fg='white',
                      selectcolor='#2b2b2b').pack(side=tk.LEFT, padx=5)
        
        # Axis selection (radio buttons)
        axis_frame = tk.Frame(control_frame, bg='#2b2b2b')
        axis_frame.pack(fill=tk.X, padx=5, pady=2)
        
        tk.Label(axis_frame, text='Axis:', bg='#2b2b2b', fg='white').pack(side=tk.LEFT)
        
        tk.Radiobutton(axis_frame, text='X Only', variable=self.axis_mode, value='x',
                      command=self.update_display, bg='#2b2b2b', fg='white',
                      selectcolor='#2b2b2b').pack(side=tk.LEFT, padx=5)
        tk.Radiobutton(axis_frame, text='Y Only', variable=self.axis_mode, value='y',
                      command=self.update_display, bg='#2b2b2b', fg='white',
                      selectcolor='#2b2b2b').pack(side=tk.LEFT, padx=5)
        tk.Radiobutton(axis_frame, text='Both X+Y', variable=self.axis_mode, value='both',
                      command=self.update_display, bg='#2b2b2b', fg='white',
                      selectcolor='#2b2b2b').pack(side=tk.LEFT, padx=5)
        
        self.animating = False
        self.update_display()
    
    def on_time_range_change(self):
        self.start_time = self.start_time_var.get()
        self.end_time = self.end_time_var.get()
        
        # Ensure end time is greater than start time
        if self.end_time <= self.start_time:
            self.end_time = self.start_time + 0.1
            self.end_time_var.set(self.end_time)
        
        # Update slider range
        self.time_slider.config(from_=self.start_time, to=self.end_time)
        
        # Clamp current time to new range
        if self.time < self.start_time:
            self.time = self.start_time
            self.time_var.set(self.time)
        elif self.time > self.end_time:
            self.time = self.end_time
            self.time_var.set(self.time)
        
        self.update_display()
    
    def on_time_change(self, value):
        self.time = float(value)
        self.update_display()
    
    def toggle_animation(self):
        self.animating = not self.animating
        self.play_button.config(text='Pause' if self.animating else 'Play')
        if self.animating:
            self.animate()
    
    def animate(self):
        if self.animating:
            self.time += 0.02 * self.speed_var.get()
            if self.time > self.end_time:
                self.time = self.start_time
            self.time_var.set(self.time)
            self.update_display()
            self.parent.after(20, self.animate)
    
    def draw_coordinate_system(self, canvas):
        """Draw coordinate system on canvas"""
        width = canvas.winfo_width()
        height = canvas.winfo_height()
        center_x, center_y = width // 2, height // 2
        
        # Draw coordinate system
        canvas.create_line(center_x, 0, center_x, height, fill='#444', width=1)
        canvas.create_line(0, center_y, width, center_y, fill='#444', width=1)
        
        return center_x, center_y, width, height
    
    def calculate_transforms(self, curve_value):
        """Calculate transformations based on current settings"""
        transform_mode = self.transform_mode.get()
        axis_mode = self.axis_mode.get()
        
        # Initialize transforms
        trans_x, trans_y = 0, 0
        rotation = 0
        scale_x, scale_y = 1.0, 1.0
        
        if transform_mode == 'translation':
            if axis_mode == 'x' or axis_mode == 'both':
                trans_x = curve_value * 100
            if axis_mode == 'y' or axis_mode == 'both':
                trans_y = curve_value * 100
        elif transform_mode == 'rotation':
            rotation = curve_value * 360
        elif transform_mode == 'scale':
            if axis_mode == 'x' or axis_mode == 'both':
                scale_x = curve_value
            if axis_mode == 'y' or axis_mode == 'both':
                scale_y = curve_value
        
        return trans_x, trans_y, rotation, scale_x, scale_y
    
    def draw_transform_info(self, canvas, curve_value, width, height):
        """Draw transformation information"""
        trans_x, trans_y, rotation, scale_x, scale_y = self.calculate_transforms(curve_value)
        
        # Draw info text
        info_y = 10
        canvas.create_text(10, info_y, anchor='nw', 
                          text=f'Time: {self.time:.2f}', 
                          fill='white', font=('Arial', 9))
        info_y += 15
        canvas.create_text(10, info_y, anchor='nw', 
                          text=f'Curve Value: {curve_value:.3f}', 
                          fill='white', font=('Arial', 9))
        info_y += 15
        
        transform_mode = self.transform_mode.get()
        axis_mode = self.axis_mode.get()
        
        if transform_mode == 'translation':
            if axis_mode == 'x' or axis_mode == 'both':
                canvas.create_text(10, info_y, anchor='nw', 
                                  text=f'Translation X: {trans_x:.1f}px', 
                                  fill='#2196F3', font=('Arial', 9))
                info_y += 12
            if axis_mode == 'y' or axis_mode == 'both':
                canvas.create_text(10, info_y, anchor='nw', 
                                  text=f'Translation Y: {trans_y:.1f}px', 
                                  fill='#03A9F4', font=('Arial', 9))
                info_y += 15
        elif transform_mode == 'rotation':
            canvas.create_text(10, info_y, anchor='nw', 
                              text=f'Rotation: {rotation:.1f}°', 
                              fill='#FF9800', font=('Arial', 9))
            info_y += 15
        elif transform_mode == 'scale':
            if axis_mode == 'x' or axis_mode == 'both':
                canvas.create_text(10, info_y, anchor='nw', 
                                  text=f'Scale X: {scale_x:.3f}x', 
                                  fill='#9C27B0', font=('Arial', 9))
                info_y += 12
            if axis_mode == 'y' or axis_mode == 'both':
                canvas.create_text(10, info_y, anchor='nw', 
                                  text=f'Scale Y: {scale_y:.3f}x', 
                                  fill='#E91E63', font=('Arial', 9))
        
        return trans_x, trans_y, rotation, scale_x, scale_y
    
    def draw_object(self, canvas, center_x, center_y, trans_x, trans_y, rotation, scale_x, scale_y):
        """Draw transformed object"""
        size = 20
        x = center_x + trans_x
        y = center_y + trans_y
        
        # Simple rotation and scaling
        cos_a = math.cos(math.radians(rotation))
        sin_a = math.sin(math.radians(rotation))
        
        corners = [
            (-size * scale_x, -size * scale_y),
            (size * scale_x, -size * scale_y),
            (size * scale_x, size * scale_y),
            (-size * scale_x, size * scale_y)
        ]
        
        # Rotate corners
        rotated_corners = []
        for cx, cy in corners:
            rx = cx * cos_a - cy * sin_a
            ry = cx * sin_a + cy * cos_a
            rotated_corners.append((x + rx, y + ry))
        
        # Draw cube
        points = []
        for corner in rotated_corners:
            points.extend(corner)
        points.extend(rotated_corners[0])  # Close the shape
        
        canvas.create_polygon(points, fill='#4CAF50', outline='white', width=2)
    
    def update_display(self):
        # Get curve value
        curve_value = self.curve_editor.curve.evaluate(self.time)
        
        # Update single canvas
        self.canvas.delete('all')
        center_x, center_y, width, height = self.draw_coordinate_system(self.canvas)
        trans_x, trans_y, rotation, scale_x, scale_y = self.draw_transform_info(self.canvas, curve_value, width, height)
        self.draw_object(self.canvas, center_x, center_y, trans_x, trans_y, rotation, scale_x, scale_y)

class HermitCurveApp:
    def __init__(self):
        self.root = tk.Tk()
        self.root.title('Unity-style Hermit Curve Editor - Enhanced')
        self.root.geometry('1200x800')
        self.root.configure(bg='#2b2b2b')
        
        # Create main paned window
        paned_window = tk.PanedWindow(self.root, orient=tk.HORIZONTAL, bg='#2b2b2b')
        paned_window.pack(fill=tk.BOTH, expand=True)
        
        # Left panel - Curve Editor
        left_frame = tk.Frame(paned_window, bg='#2b2b2b')
        paned_window.add(left_frame, width=600)
        
        tk.Label(left_frame, text='Curve Editor', bg='#2b2b2b', fg='white', 
                font=('Arial', 12, 'bold')).pack(pady=5)
        
        self.curve_editor = CurveEditor(left_frame)
        self.curve_editor.app = self # CurveEditor가 app 인스턴스를 참조할 수 있도록 설정
        
        # Right panel - Preview
        right_frame = tk.Frame(paned_window, bg='#2b2b2b')
        paned_window.add(right_frame, width=600)
        
        tk.Label(right_frame, text='Animation Preview', bg='#2b2b2b', fg='white',
                font=('Arial', 12, 'bold')).pack(pady=5)
        
        self.preview_panel = PreviewPanel(right_frame, self.curve_editor)
        
        # Menu bar
        menubar = tk.Menu(self.root)
        self.root.config(menu=menubar)
        
        file_menu = tk.Menu(menubar, tearoff=0)
        menubar.add_cascade(label='File', menu=file_menu)
        file_menu.add_command(label='Save Curve', command=self.save_curve)
        file_menu.add_command(label='Load Curve', command=self.load_curve)
        file_menu.add_separator()
        file_menu.add_command(label='Exit', command=self.root.quit)
        
        edit_menu = tk.Menu(menubar, tearoff=0)
        menubar.add_cascade(label='Edit', menu=edit_menu)
        edit_menu.add_command(label='Edit Keyframe (E)', command=self.edit_selected_keyframe)
        edit_menu.add_command(label='Toggle Tangent Mode (T)', command=self.toggle_selected_tangent)
        edit_menu.add_command(label='Reset View', command=self.reset_view)
        
        # Instructions
        instructions = """
Enhanced Controls:
• Left Click: Add keyframe or select existing
• Double-Click: Edit keyframe values
• Right Click: Delete keyframe OR toggle tangent mode (on selected)
• Middle Mouse + Drag: Pan view
• Mouse Wheel: Zoom
• Drag keyframes to move them  
• Drag red/gray tangent handles to adjust curve shape
• Delete key: Remove selected keyframe
• T key: Toggle tangent mode (Unified/Broken)
• E key: Edit selected keyframe

Tangent Modes:
• Orange keyframe: Unified tangents (parallel movement)
• Red keyframe: Broken tangents (independent movement)

Transform Types & Axis Selection:
• Choose one transform type (Translation/Rotation/Scale)
• Choose axis application (X Only/Y Only/Both X+Y)
• Preview shows the selected combination
        """
        
        info_frame = tk.Frame(left_frame, bg='#2b2b2b')
        info_frame.pack(fill=tk.X, pady=5)
        tk.Label(info_frame, text=instructions, bg='#2b2b2b', fg='#aaa',
                font=('Arial', 8), justify=tk.LEFT).pack(anchor='w', padx=10)
    
    def edit_selected_keyframe(self):
        self.curve_editor.edit_selected_keyframe()
    
    def toggle_selected_tangent(self):
        if self.curve_editor.selected_keyframe >= 0:
            self.curve_editor.toggle_tangent_mode()
    
    def reset_view(self):
        self.curve_editor.view_offset_x = 0
        self.curve_editor.view_offset_y = 0
        self.curve_editor.zoom = 50
        self.curve_editor.redraw()
    
    def update_preview(self):
        self.preview_panel.update_display()
    
# HermitCurveApp 클래스 내에서
    def save_curve(self): # 'self' 추가, file_path와 curve 인자 제거
        try:
            filename = filedialog.asksaveasfilename(
                defaultextension=".json",
                filetypes=[('JSON files', '*.json'), ('All files', '*.*')]
            )
            if filename:
                # self.curve_editor.curve는 HermitCurve 객체입니다.
                with open(filename, 'w') as f:
                    json.dump(self.curve_editor.curve.to_json(), f, indent=4)
                messagebox.showinfo('Success', f"Curve saved to {filename}")
                print(f"Curve saved to {filename}")
        except Exception as e:
            messagebox.showerror('Error', f'Failed to save curve: {str(e)}')
            print(f"Error saving curve: {e}")
    
    def load_curve(self):
        try:
            filename = filedialog.askopenfilename(
                defaultextension=".json",
                filetypes=[('JSON files', '*.json'), ('All files', '*.*')]
            )
            if filename:
                with open(filename, 'r') as f:
                    json_data = json.load(f) # JSON 파일 로드

                # JSON 데이터를 파싱하여 HermitCurve 객체를 다시 구성
                new_curve = HermitCurve()
                if "keyframes" in json_data and isinstance(json_data["keyframes"], list):
                    for kf_data in json_data["keyframes"]:
                        if isinstance(kf_data, dict):
                            # 각 키프레임 데이터 유효성 검사 및 추가
                            time = kf_data.get("time")
                            value = kf_data.get("value")
                            in_tangent = kf_data.get("in_tangent")
                            out_tangent = kf_data.get("out_tangent")
                            tangent_mode = kf_data.get("tangent_mode", 1) # 기본값 1

                            if all(v is not None for v in [time, value, in_tangent, out_tangent]):
                                new_curve.add_keyframe(time, value, in_tangent, out_tangent, tangent_mode)
                            else:
                                raise ValueError(f"Invalid keyframe data in JSON: {kf_data}")
                        else:
                            raise ValueError(f"Invalid keyframe format: {kf_data}. Expected dictionary.")
                else:
                    raise ValueError("JSON data must contain a 'keyframes' list.")

                self.curve_editor.curve = new_curve # 새로 로드된 커브로 교체
                self.curve_editor.selected_keyframe = -1 # 선택된 키프레임 초기화
                self.curve_editor.redraw() # 커브 에디터 다시 그리기
                self.update_preview() # 프리뷰 업데이트
                messagebox.showinfo('Success', f"Curve loaded from {filename}")
                print(f"Curve loaded from {filename}")

        except json.JSONDecodeError as e:
            messagebox.showerror('Error', f'Failed to load curve: Invalid JSON file. {str(e)}')
            print(f"Error loading curve: Invalid JSON file. {e}")
        except ValueError as e:
            messagebox.showerror('Error', f'Failed to load curve: Data format error. {str(e)}')
            print(f"Error loading curve: Data format error. {e}")
        except Exception as e:
            messagebox.showerror('Error', f'Failed to load curve: {str(e)}')
            print(f"Error loading curve: {e}")
    
    def get_curve_data_string(self):
        """Returns curve data as a simple string format for easy parsing"""
        lines = []
        for t, v, in_tan, out_tan, mode in self.curve_editor.curve.keyframes:
            lines.append(f"{t},{v},{in_tan},{out_tan},{mode}")
        return '\n'.join(lines)
    
    def set_curve_from_string(self, data_string):
        """Sets curve data from a string format"""
        try:
            keyframes = []
            for line in data_string.strip().split('\n'):
                if line.strip():  # Skip empty lines
                    parts = line.split(',')
                    if len(parts) >= 4:
                        t = float(parts[0])
                        v = float(parts[1])
                        in_tan = float(parts[2])
                        out_tan = float(parts[3])
                        mode = int(parts[4]) if len(parts) > 4 else 1  # Default to unified
                        keyframes.append([t, v, in_tan, out_tan, mode])
            
            self.curve_editor.curve.keyframes = keyframes
            self.curve_editor.selected_keyframe = -1
            self.curve_editor.redraw()
            self.update_preview()
            return True
        except Exception as e:
            print(f"Error parsing curve data: {e}")
            return False
    
    def run(self):
        self.root.mainloop()

if __name__ == '__main__':
    app = HermitCurveApp()
    app.run()
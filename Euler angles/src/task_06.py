import numpy as np
import math

from src.engine.animation.TrsTransformationAnimation import TrsTransformationAnimation
from src.engine.model.Cube import Cube
from src.engine.scene.AnimatedScene import AnimatedScene
from src.math.Mat4x4 import Mat4x4

if __name__ == '__main__':
    CUBE_ANIMATED_KEY = "cube_animated"

    class InterpolationScene(AnimatedScene):
        def __init__(self, **kwargs):
            super().__init__(**kwargs)

            self[CUBE_ANIMATED_KEY] = Cube(self.plt_axis, alpha=0.3, color="green")
            
            for i in range(11):  
                t = i / 10
                angle = t * 90
                
                Rx = Mat4x4.rotation_x(angle, is_radians=False)
                Ry = Mat4x4.rotation_y(angle, is_radians=False)
                Rz = Mat4x4.rotation_z(angle, is_radians=False)
                M_step = Rz * Ry * Rx
                
                step_cube = Cube(self.plt_axis, alpha=0.05, color="red", line_width=0.5)
                step_cube.transformation = M_step
                self[f"step_{i}"] = step_cube

    animated_scene = InterpolationScene(
        coordinate_rect=(-1, -1, -1, 2, 2, 2),
        title="Завдання 6: Проблема інтерполяції в зоні сингулярності"
    )
    M_end = Mat4x4.rotation_z(90, False) * Mat4x4.rotation_y(90, False) * Mat4x4.rotation_x(90, False)
    animation_lerp = TrsTransformationAnimation(
        end=M_end,            
        channel=CUBE_ANIMATED_KEY,
        frames=120,             
        repeat=False          
    )

    animated_scene.add_animations(animation_lerp)
    animated_scene.show()
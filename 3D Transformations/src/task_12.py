import numpy as np
import math

from src.engine.animation.TrsTransformationAnimation import TrsTransformationAnimation
from src.engine.model.Cube import Cube
from src.engine.scene.AnimatedScene import AnimatedScene
from src.math.Mat4x4 import Mat4x4
from src.math.Vec4 import Vec4


if __name__ == '__main__':
    CUBE_START_KEY = "cube_start"
    CUBE_DECOMPOSED_KEY = "cube_decomposed"

    class DecompositionScene(AnimatedScene):
        def __init__(self, **kwargs):
            super().__init__(**kwargs)
            self[CUBE_START_KEY] = Cube(self.plt_axis, alpha=0.1, color="blue")
            self[CUBE_DECOMPOSED_KEY] = Cube(self.plt_axis, alpha=0.2, color="grey", 
                                             line_width=1.5, line_style="-.")
    animated_scene = DecompositionScene(
        coordinate_rect=(-3, -3, -3, 5, 6, 6),
        title="Завдання 12: Декомпозиція афінної матриці («Чорна скринька»)"
    )

    T = Mat4x4.translation(-2, 4, 3)
    R = Mat4x4.rotation_y(45, is_radians=False) * Mat4x4.rotation_x(30, is_radians=False)
    S = Mat4x4.scale(1.5, 2.0, 0.5)
    M_black_box = T * R * S

    animation = TrsTransformationAnimation(
        end=M_black_box,            
        channel=CUBE_DECOMPOSED_KEY,
        frames=150,             
        repeat=False,
        apply_geometry_transformation_on_finish=True
    )

    animated_scene.add_animations(animation)
    animated_scene.show()
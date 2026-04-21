import numpy as np
import math

from src.engine.animation.TrsTransformationAnimation import TrsTransformationAnimation
from src.engine.model.Cube import Cube
from src.engine.scene.AnimatedScene import AnimatedScene
from src.math.Mat4x4 import Mat4x4
from src.math.Vec4 import Vec4
from src.engine.model.Model import Model
from src.base.points import draw_point


class PointModel(Model):
    def __init__(self, plt_axis, coords, color="fuchsia", size=50):
        super().__init__(plt_axis, *coords) 
        self.point_color = color
        self.point_size = size

    def draw_model(self):
        p = self.transformed_geometry[0]
        draw_point(self.plt_axis, p.xyz, color=self.point_color, size=self.point_size)

        
if __name__ == '__main__':
    CUBE_START_KEY = "cube_start"
    CUBE_ANIMATED_KEY = "cube_animated"
    PIVOT_KEY = "pivot_point"

    class CubeScene(AnimatedScene):
        def __init__(self, **kwargs):
            super().__init__(**kwargs)
            self[CUBE_START_KEY] = Cube(self.plt_axis, alpha=0.1, color="blue")
            self[CUBE_ANIMATED_KEY] = Cube(self.plt_axis, alpha=0.1, color="grey", line_style="-.")

            self[PIVOT_KEY] = PointModel(self.plt_axis, (2.0, 0.0, 3.0), color="fuchsia", size=50)

    animated_scene = CubeScene(
        coordinate_rect=(-2, -2, -2, 5, 5, 7),
        title="Завдання 6: Обертання навколо осі Oz з pivot (2,0,3), переміщення на (-1,2,4)"
    )

    pivot_p = (2, 0, 3)
    T1 = Mat4x4.translation(-pivot_p[0], -pivot_p[1], -pivot_p[2])
    Ry = Mat4x4.rotation_y(45, is_radians=False)
    T2 = Mat4x4.translation(pivot_p[0], pivot_p[1], pivot_p[2])
    
    M_pivot = T2 * Ry * T1
    T_final = Mat4x4.translation(-1, 2, 4)
    M_total = T_final * M_pivot

    animation_move = TrsTransformationAnimation(
        end=M_total,            
        channel=CUBE_ANIMATED_KEY,
        frames=120,             
        repeat=False          
    )
    animated_scene.add_animations(animation_move)
    animated_scene.show()
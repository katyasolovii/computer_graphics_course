import numpy as np
import math

from src.engine.animation.TrsTransformationAnimation import TrsTransformationAnimation
from src.engine.model.SimplePolygon import SimplePolygon
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
    RECT_START_KEY = "rect_start"
    RECT_ANIMATED_KEY = "rect_animated"
    PIVOT_KEY = "pivot_point"

    v = [(1, 2, 0), (4, 2, 0), (4, 5, 0), (1, 5, 0)]

    class RectScene(AnimatedScene):
        def __init__(self, **kwargs):
            super().__init__(**kwargs)
            self[RECT_START_KEY] = SimplePolygon(self.plt_axis, *v, 
                                                 color="blue", alpha=0.2)
            self[RECT_ANIMATED_KEY] = SimplePolygon(self.plt_axis, *v, 
                                                    color="grey", alpha=0.4, line_style="-.")
            self[PIVOT_KEY] = PointModel(self.plt_axis, (3.0, 3.0, 0.0), color="fuchsia", size=50)


    animated_scene = RectScene(
        coordinate_rect=(-1, -1, -4, 7, 7, 4),
        title="Завдання 9: Зміна перспективи (Rot Y 60°, Rot X 30°) \n навколо pivot (3,3,0)"
    )
    pivot_p = (3, 3, 0)
    
    T_to_origin = Mat4x4.translation(-pivot_p[0], -pivot_p[1], -pivot_p[2])
    T_back = Mat4x4.translation(pivot_p[0], pivot_p[1], pivot_p[2])
    Ry = Mat4x4.rotation_y(60, is_radians=False)
    M_pivot_y = T_back * Ry * T_to_origin
    Rx = Mat4x4.rotation_x(30, is_radians=False)
    M_pivot_x = T_back * Rx * T_to_origin
    M_total = M_pivot_x * M_pivot_y

    animation = TrsTransformationAnimation(
        end=M_total,            
        channel=RECT_ANIMATED_KEY,
        frames=150,             
        repeat=False,
        apply_geometry_transformation_on_finish=True
    )

    animated_scene.add_animations(animation)
    animated_scene.show()
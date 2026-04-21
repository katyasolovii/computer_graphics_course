import numpy as np
import math

from src.engine.animation.TrsTransformationAnimation import TrsTransformationAnimation
from src.engine.model.Model import Model
from src.engine.model.SimplePolygon import SimplePolygon
from src.engine.scene.AnimatedScene import AnimatedScene
from src.math.Mat4x4 import Mat4x4
from src.math.Vec4 import Vec4


class Tetrahedron(Model):
    def __init__(self, plt_axis, color="grey", alpha=1.0):
        super().__init__(plt_axis)
        self.polygons = []
        v = [[0, 0, 0], [1, 0, 0], [0, 1, 0], [0, 0, 1]]
        faces_indices = [[0, 1, 2], [0, 1, 3], [0, 2, 3], [1, 2, 3]]
        for idx in faces_indices:
            face_vertices = [v[i] for i in idx]
            self.polygons.append(SimplePolygon(self.plt_axis, *face_vertices, color=color, alpha=alpha))

    def draw_model(self):
        for polygon in self.polygons:
            polygon.transformation = self.transformation
            polygon.draw()

    def apply_transformation_to_geometry(self):
        super().apply_transformation_to_geometry()
        for polygon in self.polygons:
            polygon.apply_transformation_to_geometry()

if __name__ == '__main__':
    TETRA_START_KEY = "tetra_start"
    TETRA_ANIMATED_KEY = "tetra_animated"

    class TetraScene(AnimatedScene):
        def __init__(self, **kwargs):
            super().__init__(**kwargs)
            self[TETRA_START_KEY] = Tetrahedron(self.plt_axis, color="blue", alpha=0.1)
            self[TETRA_ANIMATED_KEY] = Tetrahedron(self.plt_axis, color="grey", alpha=0.4)

    animated_scene = TetraScene(
        coordinate_rect=(-2, -1, -2, 4, 4, 4),
        title="Завдання 13: Внутрішні обертання та локальна система координат"
    )


    M1 = Mat4x4.rotation_x(45, is_radians=False)
    T_local = Mat4x4.translation(0, 2, 0)
    M2 = M1 * T_local 
    R_local_z = Mat4x4.rotation_z(30, is_radians=False)
    M_total = M2 * R_local_z


    animation = TrsTransformationAnimation(
        end=M_total,
        channel=TETRA_ANIMATED_KEY,
        frames=150,
        repeat=False,
        apply_geometry_transformation_on_finish=True
    )
    
    animated_scene.add_animations(animation)
    animated_scene.show()
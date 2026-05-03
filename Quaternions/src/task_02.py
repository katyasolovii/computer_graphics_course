import numpy as np
import math

from src.engine.animation.QuaternionAnimation import QuaternionAnimation
from src.engine.model.Model import Model
from src.engine.model.SimplePolygon import SimplePolygon  
from src.engine.scene.AnimatedScene import AnimatedScene
from src.math.Quaternion import Quaternion
from src.math.Vec3 import Vec3

TETRA_KEY = "tetrahedron"

class Tetrahedron(Model):
    def __init__(self, plt_axis, color="lightgreen", alpha=0.4):
        super().__init__(plt_axis)
        self.polygons = []
        
        v = [
            [0, 0, 0], 
            [1, 0, 0], 
            [0, 1, 0], 
            [0, 0, 1]  
        ]

        faces_indices = [
            [0, 1, 2], 
            [0, 1, 3], 
            [0, 2, 3], 
            [1, 2, 3] 
        ]

        for idx in faces_indices:
            face_vertices = [v[i] for i in idx]
            self.polygons.append(
                SimplePolygon(self.plt_axis, 
                              *face_vertices, 
                              color=color, 
                              alpha=alpha)
            )

    def draw_model(self):
        for polygon in self.polygons:
            polygon.transformation = self.transformation
            polygon.pivot(self._pivot)
            polygon.draw()

    def apply_transformation_to_geometry(self):
        super().apply_transformation_to_geometry()
        for polygon in self.polygons:
            polygon.apply_transformation_to_geometry()

class TetraScene(AnimatedScene):
    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        tetra = Tetrahedron(self.plt_axis, color="lightgreen", alpha=0.5)
        self[TETRA_KEY] = tetra
        
        tetra.show_pivot()
        tetra.show_local_frame()

if __name__ == '__main__':
    animated_scene = TetraScene(
        title="Завдання 2: Композиція обертань (X:45°, Y:30°)",
        image_size=(10, 10),
        coordinate_rect=(-1, -1, -1, 1, 1, 1),
    )

    q1 = Quaternion.rotation_x(math.radians(45))
    q2 = Quaternion.rotation_y(math.radians(30))
    q_total = q2 * q1

    animation_quat = QuaternionAnimation(
        end_quaternion=q_total,
        channel=TETRA_KEY,
        frames=150,  
        repeat=False    
    )

    animated_scene.add_animations(animation_quat)
    animated_scene.show()
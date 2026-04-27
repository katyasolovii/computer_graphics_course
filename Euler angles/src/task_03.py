import numpy as np
import math

from src.engine.animation.TrsTransformationAnimation import TrsTransformationAnimation
from src.engine.model.Cube import Cube
from src.engine.scene.AnimatedScene import AnimatedScene
from src.math.Mat4x4 import Mat4x4

if __name__ == '__main__':
    CUBE_XYZ_KEY = "cube_xyz"
    CUBE_ZXZ_KEY = "cube_zxz"

    class EulerComparisonScene(AnimatedScene):
        def __init__(self, **kwargs):
            super().__init__(**kwargs)
            cube_xyz = Cube(self.plt_axis, alpha=0.2, color="blue")
            self[CUBE_XYZ_KEY] = cube_xyz
            
            cube_zxz = Cube(self.plt_axis, alpha=0.2, color="red")
            self[CUBE_ZXZ_KEY] = cube_zxz


    animated_scene = EulerComparisonScene(
        coordinate_rect=(-1, -1, -1, 3, 3, 3),
        title="Завдання 3: Порівняння конвенцій XYZ та ZXZ (45, 30, 60) \n Синій: XYZ, Червоний: ZXZ"
    )

    phi, theta, psi = 45, 30, 60

    Rx = Mat4x4.rotation_x(phi, is_radians=False)
    Ry = Mat4x4.rotation_y(theta, is_radians=False)
    Rz = Mat4x4.rotation_z(psi, is_radians=False)
    M_xyz = Rz * Ry * Rx

    Rz_phi = Mat4x4.rotation_z(phi, is_radians=False)
    Rx_theta = Mat4x4.rotation_x(theta, is_radians=False)
    Rz_psi = Mat4x4.rotation_z(psi, is_radians=False)
    M_zxz = Rz_psi * Rx_theta * Rz_phi

    anim_xyz = TrsTransformationAnimation(
        end=M_xyz,            
        channel=CUBE_XYZ_KEY,
        frames=150,             
        repeat=False          
    )

    anim_zxz = TrsTransformationAnimation(
        end=M_zxz,            
        channel=CUBE_ZXZ_KEY,
        frames=150,             
        repeat=False          
    )

    animated_scene.add_animations(anim_xyz, anim_zxz)
    animated_scene.show()
import numpy as np

from src.engine.animation.QuaternionAnimation import QuaternionAnimation
from src.engine.model.Cube import Cube
from src.engine.scene.AnimatedScene import AnimatedScene
from src.math.Quaternion import Quaternion

CUBE_KEY = "cube"

class AnimScene(AnimatedScene):
    def __init__(self, **kwargs):
        super().__init__(**kwargs)

        cube = Cube(self.plt_axis, alpha=0.2, color='skyblue')
        self[CUBE_KEY] = cube
        
        cube.show_pivot()
        cube.show_local_frame()

if __name__ == '__main__':
    animated_scene = AnimScene(
        title="Завдання 0: Від осі та кута до кватерніона", 
        image_size=(10, 10),
        coordinate_rect=(-1, -1, -1, 1, 1, 1),
    )

    theta = np.radians(60) 
    # Одиничний вектор u = 1/sqrt(3) * (1, 1, 1)
    u_vector = np.array([1.0, 1.0, 1.0]) / np.sqrt(3.0)

    #  q = cos(theta/2) + u * sin(theta/2)
    q_task_0 = Quaternion.rotation(theta, u_vector)

    animation_quat = QuaternionAnimation(
        end_quaternion=q_task_0,
        channel=CUBE_KEY
    )
    
    animated_scene.add_animations(
        animation_quat,
    )

    animated_scene.show()
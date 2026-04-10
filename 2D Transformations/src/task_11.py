import numpy as np
from src.engine.model.Polygon import Polygon
from src.engine.scene.Scene import Scene

if __name__ == '__main__':

    ORIGINAL_KEY = "local_rect"
    TRANSFORMED_KEY = "global_rect"


    class Task_11(Scene):

        def __init__(self, **kwargs):
            super().__init__(**kwargs)

            T = np.array([
                [2.934, -0.416, 2.000],
                [0.624,  1.956, 3.400],
                [0,      0,     1]
            ])
            # оберненна матриця для перетворення глобальних координат в локальні
            T_inv = np.linalg.inv(T)
            global_pts = [
                (2, 3.4),
                (4.9, 4),
                (4.5, 6),
                (1.6, 5.4)
            ]

            local_pts = []
            for x, y in global_pts:
                vec = np.array([x, y, 1])
                res = res = np.dot(T_inv, vec)
                local_pts.append((res[0], res[1]))

            local_rect = Polygon()
            self[ORIGINAL_KEY] = local_rect

            local_rect.set_geometry(
                local_pts[0][0], local_pts[0][1],
                local_pts[1][0], local_pts[1][1],
                local_pts[2][0], local_pts[2][1],
                local_pts[3][0], local_pts[3][1],
            )
            global_rect = Polygon()
            self[TRANSFORMED_KEY] = global_rect
            global_rect.set_geometry(
                2, 3.4,
                4.9, 4,
                4.5, 6,
                1.6, 5.4
            )


    def setup_scene(scene):
        local_rect = scene[ORIGINAL_KEY]
        local_rect["color"] = "blue"
        local_rect["vertices_show"] = True

        global_rect = scene[TRANSFORMED_KEY]
        global_rect["color"] = "red"
        global_rect["vertices_show"] = True


    scene = Task_11(
        image_size=(7, 7),
        coordinate_rect=(-1, -1, 6, 7),
        title="Завдання 11: Відновлення початкового зображення",
        grid_show=True,
        axis_show=True
    )

    scene.add_frames(setup_scene)
    scene.show()
import numpy as np
from src.engine.model.Polygon import Polygon
from src.engine.scene.Scene import Scene

if __name__ == '__main__':

    ORIGINAL_KEY = "square"
    TRANSFORMED_KEY = "square_transformed"
    PIVOT_KEY = "pivot"

    class Task_14(Scene):

        def __init__(self, **kwargs):
            super().__init__(**kwargs)

            T = np.array([
                [1.732, -1, 5],
                [1, 1.732, -3],
                [0, 0, 1]
            ])

            square = Polygon()
            self[ORIGINAL_KEY] = square
            square.set_geometry(
                0, 0,
                1, 0,
                1, 1,
                0, 1
            )
            
            trans_square = Polygon()
            self[TRANSFORMED_KEY] = trans_square

            points = [(0,0),(1,0),(1,1),(0,1)]
            new_pts = []

            for x, y in points:
                vec = np.array([x, y, 1])
                res = np.dot(T, vec)
                new_pts.append((res[0], res[1]))

            trans_square.set_geometry(
                new_pts[0][0], new_pts[0][1],
                new_pts[1][0], new_pts[1][1],
                new_pts[2][0], new_pts[2][1],
                new_pts[3][0], new_pts[3][1],
            )
            pivot = (1, 1)
            pivot_obj = Polygon()
            self[PIVOT_KEY] = pivot_obj
            px, py = pivot
            pivot_obj.set_geometry(px, py)


    def setup_scene(scene):
        square = scene[ORIGINAL_KEY]
        square["color"] = "blue"
        square["vertices_show"] = True

        trans = scene[TRANSFORMED_KEY]
        trans["color"] = "red"
        trans["vertices_show"] = True

        pivot = scene[PIVOT_KEY]
        pivot["color"] = "orange"
        pivot["vertices_show"] = True
        pivot["vertex_color"] = "fuchsia"
        pivot["vertex_size"] = 30


    scene = Task_14(
        image_size=(6, 6),
        coordinate_rect=(-1, -4, 8, 3),
        title="Завдання 14: Розкладання матриці трансформацій з Pivot(1, 1)",
        grid_show=True,
        axis_show=True
    )

    scene.add_frames(setup_scene)
    scene.show()
import numpy as np

from src.engine.model.Polygon import Polygon
from src.engine.scene.Scene import Scene

if __name__ == '__main__':
    ORIGINAL_KEY = "square"
    TRANSFORMED_KEY = "square_transformed"

    class Task_1_6(Scene):
        def __init__(self, **kwargs):
            super().__init__(**kwargs)
            for key in [ORIGINAL_KEY, TRANSFORMED_KEY]:
                square = Polygon()
                self[key] = square
                square.set_geometry(
                    0, 0,
                    1, 0,
                    1, 1,
                    0, 1
                )

    def apply_style(scene):
        square = scene[ORIGINAL_KEY]
        square["color"] = "blue"
        square["vertices_show"] = True
        square["vertex_color"] = "black"
        
        trans = scene[TRANSFORMED_KEY]
        trans["color"] = "red"
        trans["vertices_show"] = True
        trans["vertex_color"] = "black"
        return trans

    def frame1(scene):
        t = apply_style(scene)
        t.rotation, t.translation = np.radians(30), (2, 3)
    
    def frame2(scene):
        t = apply_style(scene)
        t.scale, t.rotation = (2, 1), np.radians(45)

    def frame3(scene):
        t = apply_style(scene)
        t.rotation, t.translation = np.radians(90), (2, 3)

    def frame4(scene):
        t = apply_style(scene)
        t.scale, t.rotation = (1, 3), np.radians(60)

    def frame5(scene):
        t = apply_style(scene)
        t.scale, t.translation = (2, 2), (2, -2)
        
    def frame6(scene):
        t = apply_style(scene)
        t.scale, t.rotation, t.translation = (1, 3), np.radians(60), (2, 3)

    tasks = [
        ("Завдання 1: Композиція трансформацій", frame1, (-1, -1, 5, 5)),
        ("Завдання 2: Розтяг і поворот", frame2, (-1, -1, 3, 3)),
        ("Завдання 3: Переміщення і масштабування", frame3, (-1, -1, 5, 5)),
        ("Завдання 4: Розтяг і поворот", frame4, (-4, -1, 4, 4)),
        ("Завдання 5: Переміщення і масштабування", frame5, (-1, -3, 6, 6)),
        ("Завдання 6: Композиція трьох трансформацій", frame6, (-2, -1, 5, 6)),
    ]

    for title, frame_func, rect in tasks:
        scene = Task_1_6(
            image_size=(8, 8), 
            coordinate_rect=rect, 
            title=title,
            grid_show=True, 
            axis_show=True, 
            base_axis_show=False,
            axis_color=("red", "green"), 
            axis_line_style="-."
        )
        scene.add_frames(frame_func)
        scene.show()






# if __name__ == '__main__':
#     ORIGINAL_KEY = "square"
#     TRANSFORMED_KEY = "square_transformed"


#     class Task_1_6(Scene):

#         def __init__(self, **kwargs):
#             super().__init__(**kwargs)

#             square = Polygon()
#             self[ORIGINAL_KEY] = square
#             square.set_geometry(
#                 0, 0,
#                 1, 0,
#                 1, 1,
#                 0, 1
#             )

#             trans_square = Polygon()
#             self[TRANSFORMED_KEY] = trans_square
#             trans_square.set_geometry(
#                 0, 0,
#                 1, 0,
#                 1, 1,
#                 0, 1
#             )


#     def frame1(scene):
#         square = scene[ORIGINAL_KEY]
#         square["color"] = "blue"  # колір ліній
#         square["vertices_show"] = True  # показувати вершини
#         square["vertex_color"] = "black"  # колір вершин

#         trans = scene[TRANSFORMED_KEY]
#         trans["color"] = "red"
#         trans["vertices_show"] = True
#         trans["vertex_color"] = "black"
#         trans.rotation = (np.radians(30))  # поворот
#         trans.translation = (2, 3)  # перенесення
    
#     def frame2(scene):
#         square = scene[ORIGINAL_KEY]
#         square["color"] = "blue"  # колір ліній
#         square["vertices_show"] = True  # показувати вершини
#         square["vertex_color"] = "black"  # колір вершин

#         trans = scene[TRANSFORMED_KEY]
#         trans["color"] = "red"
#         trans["vertices_show"] = True
#         trans["vertex_color"] = "black"
#         trans.scale = (2, 1)  # розтяг по Ox у 2 рази
#         trans.rotation = (np.radians(45))  # поворот 

#     def frame3(scene):
#         square = scene[ORIGINAL_KEY]
#         square["color"] = "blue"  # колір ліній
#         square["vertices_show"] = True  # показувати вершини
#         square["vertex_color"] = "black"  # колір вершин

#         trans = scene[TRANSFORMED_KEY]
#         trans["color"] = "red"
#         trans["vertices_show"] = True
#         trans["vertex_color"] = "black"
#         trans.rotation = (np.radians(90))  # поворот 
#         trans.translation = (2, 3)  # перенесення

#     def frame4(scene):
#         square = scene[ORIGINAL_KEY]
#         square["color"] = "blue"  # колір ліній
#         square["vertices_show"] = True  # показувати вершини
#         square["vertex_color"] = "black"  # колір вершин

#         trans = scene[TRANSFORMED_KEY]
#         trans["color"] = "red"
#         trans["vertices_show"] = True
#         trans["vertex_color"] = "black"
#         trans.scale = (1, 3)  # розтяг по Oy у 3 рази
#         trans.rotation = (np.radians(60))  # поворот 

#     def frame5(scene):
#         square = scene[ORIGINAL_KEY]
#         square["color"] = "blue"  # колір ліній
#         square["vertices_show"] = True  # показувати вершини
#         square["vertex_color"] = "black"  # колір вершин

#         trans = scene[TRANSFORMED_KEY]
#         trans["color"] = "red"
#         trans["vertices_show"] = True
#         trans["vertex_color"] = "black"
#         trans.scale = (2, 2) # масштабування у 2 рази
#         trans.translation = (2, -2)  # перенесення

#     def frame6(scene):
#         square = scene[ORIGINAL_KEY]
#         square["color"] = "blue"  # колір ліній
#         square["vertices_show"] = True  # показувати вершини
#         square["vertex_color"] = "black"  # колір вершин

#         trans = scene[TRANSFORMED_KEY]
#         trans["color"] = "red"
#         trans["vertices_show"] = True
#         trans["vertex_color"] = "black"
#         trans.scale = (1, 3)  # розтяг по Oy у 3 рази
#         trans.rotation = (np.radians(60))  # поворот 
#         trans.translation = (2, 3)  # перенесення

#     scene_1 = Task_1_6(
#         image_size=(8, 8), 
#         coordinate_rect=(-1, -1, 6, 6), 
#         title="Завдання 1: Композиція трансформацій",
#         grid_show=True,
#         base_axis_show=False, 
#         axis_show=True,  
#         axis_color=("red", "green"), 
#         axis_line_style="-." 
#     )

#     scene_2 = Task_1_6(
#         image_size=(8, 8),  
#         coordinate_rect=(-1, -1, 6, 6),  
#         title="Завдання 2: Розтяг і поворот",  
#         grid_show=True,  
#         base_axis_show=False, 
#         axis_show=True, 
#         axis_color=("red", "green"),  
#         axis_line_style="-." 
#     )

#     scene_3 = Task_1_6(
#         image_size=(8, 8),  
#         coordinate_rect=(-1, -1, 6, 6),  
#         title="Завдання 3: Поворот і перенесення",  
#         grid_show=True,  
#         base_axis_show=False, 
#         axis_show=True, 
#         axis_color=("red", "green"),  
#         axis_line_style="-." 
#     )

#     scene_4 = Task_1_6(
#         image_size=(8, 8),  
#         coordinate_rect=(-4, -1, 6, 6),  
#         title="Завдання 4: Розтяг і поворот",  
#         grid_show=True,  
#         base_axis_show=False, 
#         axis_show=True, 
#         axis_color=("red", "green"),  
#         axis_line_style="-." 
#     )

#     scene_5 = Task_1_6(
#         image_size=(8, 8),  
#         coordinate_rect=(-1, -4, 6, 6),
#         title="Завдання 5: Переміщення і масштабування",
#         grid_show=True,  
#         base_axis_show=False, 
#         axis_show=True, 
#         axis_color=("red", "green"),  
#         axis_line_style="-." 
#     )
    
#     scene_6 = Task_1_6(
#         image_size=(8, 8),  
#         coordinate_rect=(-1, -1, 6, 6),
#         title="Завдання 6:  Композиція трьох трансформацій",
#         grid_show=True,  
#         base_axis_show=False, 
#         axis_show=True, 
#         axis_color=("red", "green"),  
#         axis_line_style="-." 
#     )

#     scene_1.add_frames(frame1)
#     scene_1.show()
#     scene_2.add_frames(frame2)
#     scene_2.show()
#     scene_3.add_frames(frame3)
#     scene_3.show()
#     scene_4.add_frames(frame4)
#     scene_4.show()
#     scene_5.add_frames(frame5)
#     scene_5.show()
#     scene_6.add_frames(frame6)
#     scene_6.show()
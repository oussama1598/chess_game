"""Export the six runtime chess pieces from KillGorack's CC0 Blender scene.

Run with:
  blender -b "wood chess board.blend" --python scripts/export_regulation_pieces.py -- assets/models
"""

from pathlib import Path
import sys

import bmesh
import bpy


PIECES = {
    "pawn": ("White_Pawn", 0.82, 0.53),
    "rook": ("White_Rook", 0.96, 0.54),
    "knight": ("White_Knight", 1.06, 0.61),
    "bishop": ("White_Bishop", 1.16, 0.55),
    "queen": ("White_Queen", 1.34, 0.57),
    "king": ("White_King", 1.48, 0.59),
}


def output_directory() -> Path:
    arguments = sys.argv[sys.argv.index("--") + 1:] if "--" in sys.argv else []
    if len(arguments) != 1:
        raise RuntimeError("Expected one output-directory argument after --")
    result = Path(arguments[0]).resolve()
    result.mkdir(parents=True, exist_ok=True)
    return result


def standardized_mesh(source: bpy.types.Object, target_height: float,
                      target_width: float) -> bpy.types.Mesh:
    dependency_graph = bpy.context.evaluated_depsgraph_get()
    evaluated = source.evaluated_get(dependency_graph)
    mesh = bpy.data.meshes.new_from_object(evaluated, depsgraph=dependency_graph)

    world_vertices = [source.matrix_world @ vertex.co for vertex in mesh.vertices]
    min_x = min(vertex.x for vertex in world_vertices)
    max_x = max(vertex.x for vertex in world_vertices)
    min_y = min(vertex.y for vertex in world_vertices)
    max_y = max(vertex.y for vertex in world_vertices)
    min_z = min(vertex.z for vertex in world_vertices)
    max_z = max(vertex.z for vertex in world_vertices)
    center_x = (min_x + max_x) * 0.5
    center_y = (min_y + max_y) * 0.5
    horizontal_scale = target_width / max(max_x - min_x, max_y - min_y)
    vertical_scale = target_height / (max_z - min_z)

    for vertex, world_position in zip(mesh.vertices, world_vertices):
        vertex.co.x = (world_position.x - center_x) * horizontal_scale
        vertex.co.y = (world_position.y - center_y) * horizontal_scale
        vertex.co.z = (world_position.z - min_z) * vertical_scale

    mesh.materials.clear()
    editable = bmesh.new()
    editable.from_mesh(mesh)
    bmesh.ops.triangulate(editable, faces=list(editable.faces))
    bmesh.ops.recalc_face_normals(editable, faces=list(editable.faces))
    editable.to_mesh(mesh)
    editable.free()
    mesh.validate(clean_customdata=False)
    mesh.update(calc_edges=True)
    return mesh


def export_piece(piece_name: str, source_name: str, target_height: float,
                 target_width: float, destination: Path) -> None:
    bpy.ops.object.select_all(action="DESELECT")
    source = bpy.data.objects[source_name]
    mesh = standardized_mesh(source, target_height, target_width)
    exported = bpy.data.objects.new(f"runtime_{piece_name}", mesh)
    bpy.context.scene.collection.objects.link(exported)
    exported.select_set(True)
    bpy.context.view_layer.objects.active = exported

    output_path = destination / f"{piece_name}_low.obj"
    bpy.ops.wm.obj_export(
        filepath=str(output_path),
        check_existing=False,
        export_selected_objects=True,
        apply_modifiers=True,
        export_uv=False,
        export_normals=True,
        export_colors=False,
        export_materials=False,
        export_triangulated_mesh=True,
        forward_axis="NEGATIVE_Z",
        up_axis="Y",
    )
    print(f"Exported {piece_name}: height={target_height:.3f}, max width={target_width:.3f}")

    bpy.data.objects.remove(exported, do_unlink=True)
    bpy.data.meshes.remove(mesh)


destination = output_directory()
for name, (source_object, height, width) in PIECES.items():
    export_piece(name, source_object, height, width, destination)

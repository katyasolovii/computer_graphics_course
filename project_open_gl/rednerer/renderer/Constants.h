//
// Created by Andrii Krenevych on 04.06.2025.
//

#ifndef CONSTANTS_H
#define CONSTANTS_H

#define GLSL_VERSION "#version 330 core\n"

#define DEFAULT_NODE_NAME "default"
#define DEFAULT_CAMERA_NAME "defaultCamera"
#define DEFAULT_LIGHT_NAME            "defaultLight"
#define DEFAULT_DIRECTIONAL_LIGHT_NAME "defaultDirectionalLight"
#define DEFAULT_POINT_LIGHT_NAME       "defaultPointLight"
#define DEFAULT_SPOT_LIGHT_NAME        "defaultSpotLight"
#define ROOT_NODE_NAME    "RootNode"

#define DEFAULT_MATERIAL_NAME "default_material"

#define ATTRIB_POSITION    "aPosition"
#define ATTRIB_NORMAL      "aNormal"
#define ATTRIB_TEXTURE_UV  "aTexure"
#define ATTRIB_TANGENT     "aTangent"
#define ATTRIB_BITANGENT   "aBitangent"
#define ATTRIB_COLOR       "aColor"

#define UNIFORM_COLOR            "uColor"
#define UNIFORM_VIEW_POSITION    "uViewPos"

// Directional light
#define UNIFORM_LIGHT_DIRECTION  "uLightDir"
#define UNIFORM_LIGHT_INTENSITY  "uLightIntensity"
#define UNIFORM_LIGHT_COLOR      "uLightColor"

// Point light
#define UNIFORM_POINT_LIGHT_POSITION   "uPointLightPos"
#define UNIFORM_POINT_LIGHT_COLOR      "uPointLightColor"
#define UNIFORM_POINT_LIGHT_INTENSITY  "uPointLightIntensity"
#define UNIFORM_POINT_LIGHT_CONSTANT   "uPointLightConstant"
#define UNIFORM_POINT_LIGHT_LINEAR     "uPointLightLinear"
#define UNIFORM_POINT_LIGHT_QUADRATIC  "uPointLightQuadratic"

// Spot light
#define UNIFORM_SPOT_LIGHT_POSITION      "uSpotLightPos"
#define UNIFORM_SPOT_LIGHT_DIRECTION     "uSpotLightDir"
#define UNIFORM_SPOT_LIGHT_COLOR         "uSpotLightColor"
#define UNIFORM_SPOT_LIGHT_INTENSITY     "uSpotLightIntensity"
#define UNIFORM_SPOT_LIGHT_CUT_OFF       "uSpotLightCutOff"
#define UNIFORM_SPOT_LIGHT_OUTER_CUT_OFF "uSpotLightOuterCutOff"

///// UNIFORMS ///////

// Matrices
#define UNIFORM_MODEL      "uModel"
#define UNIFORM_VIEW       "uView"
#define UNIFORM_PROJECTION "uProjection"
#define UNIFORM_MVP        "uMVP"

/* TEXTURES */
#define TEXTURE_DIFFUSE        "uDiffuseMap"
#define TEXTURE_SPECULAR       "uSpecularMap"
#define TEXTURE_AMBIENT        "uAmbientMap"
#define TEXTURE_NORMAL         "uNormalMap"
#define TEXTURE_EMISSIVE       "uEmissiveMap"
#define TEXTURE_HEIGHT         "uHeightMap"
#define TEXTURE_SHININESS      "uShininessMap"
#define TEXTURE_OPACITY        "uOpacityMap"
#define TEXTURE_DISPLACEMENT   "uDisplacementMap"
#define TEXTURE_LIGHTMAP       "uLightmapMap"
#define TEXTURE_REFLECTION     "uReflectionMap"
#define TEXTURE_BASE_COLOR     "uBaseColorMap"        // PBR
#define TEXTURE_METALNESS      "uMetalnessMap"        // PBR
#define TEXTURE_ROUGHNESS      "uRoughnessMap"        // PBR
#define TEXTURE_AO             "uAmbientOcclusionMap" // PBR

#define MATERIAL_BASE_COLOR          "uBaseColor"        // PBR
#define MATERIAL_COLOR_DIFFUSE       "uColorDiffuse"
#define MATERIAL_COLOR_SPECULAR      "uColorSpecular"
#define MATERIAL_COLOR_AMBIENT       "uColorAmbient"
#define MATERIAL_COLOR_EMISSIVE      "uColorEmissive"
#define MATERIAL_COLOR_TRANSPARENT   "uColorTransparent"
#define MATERIAL_COLOR_REFLECTIVE    "uColorReflective"

#define MATERIAL_OPACITY             "uOpacity"
#define MATERIAL_SHININESS           "uShininess"
#define MATERIAL_SHININESS_STRENGTH  "uShininessStrength"
#define MATERIAL_REFRACTI            "uRefracti"         // індекс заломлення
#define MATERIAL_TWO_SIDED           "uTwoSided"

#define MATERIAL_NAME                "uName"
#define MATERIAL_METALLIC_FACTOR     "uMetallic"
#define MATERIAL_ROUGHNESS_FACTOR    "uRoughness"
#define MATERIAL_AO_FACTOR           "uAmbientOcclusion"



#endif //CONSTANTS_H

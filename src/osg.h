/*
 *  osg.h
 *  Boeing Demo
 *
 *  Created by WATCH on 10/6/09.
 *  Copyright 2009 Iowa State University. All rights reserved.
 *
 */

// Put all C++ headers inside the __cplusplus guard
#ifdef __cplusplus

// Common includes
#include <iostream>
#include <sstream>
#include <vector>
#include <map>

// Bullet includes
#include <BulletDynamics/btBulletDynamicsCommon.h>

// OSG Bullet
#include "osgbBullet/BoundingCone.h"
#include "osgbBullet/BoundingCylinder.h"
#include "osgbBullet/Chart.h"
#include "osgbBullet/ColladaUtils.h"
#include "osgbBullet/CollectVerticesVisitor.h"
#include "osgbBullet/CollisionShapes.h"
#include "osgbBullet/ComputeCylinderVisitor.h"
#include "osgbBullet/ComputeTriMeshVisitor.h"
#include "osgbBullet/CreationRecord.h"
#include "osgbBullet/Export.h"
#include "osgbBullet/GLDebugDrawer.h"
#include "osgbBullet/MotionState.h"
#include "osgbBullet/OSGToCollada.h"
#include "osgbBullet/PhysicsState.h"
#include "osgbBullet/PhysicsThread.h"
#include "osgbBullet/RefCollisionShape.h"
#include "osgbBullet/RefRigidBody.h"
#include "osgbBullet/RigidBodyAnimation.h"
#include "osgbBullet/TripleBuffer.h"
#include "osgbBullet/Utils.h"
#include "osgbBullet/Version.h"

// OSG Works
#include "osgwControls/Export.h"
#include "osgwControls/SliderControl.h"

#include "osgwTools/AbsoluteModelTransform.h"
#include "osgwTools/CameraConfigObject.h"
#include "osgwTools/DecimatorOp.h"
#include "osgwTools/Export.h"
#include "osgwTools/FindNamedNode.h"
#include "osgwTools/GeometryModifier.h"
#include "osgwTools/GeometryOperation.h"
#include "osgwTools/Grid.h"
#include "osgwTools/InsertRemove.h"
#include "osgwTools/ParallelVisitor.h"
#include "osgwTools/Quat.h"
#include "osgwTools/ReadFile.h"
#include "osgwTools/ReducerOp.h"
#include "osgwTools/RefID.h"
#include "osgwTools/RemoveData.h"
#include "osgwTools/ScreenCapture.h"
#include "osgwTools/SimplifierOp.h"
#include "osgwTools/Version.h"


// OSG pch
#include <OpenThreads/Barrier>
#include <OpenThreads/Condition>
#include <OpenThreads/Exports>
#include <OpenThreads/Mutex>
#include <OpenThreads/ScopedLock>
#include <OpenThreads/Thread>
#include <OpenThreads/ReentrantMutex>

#include <osg/AlphaFunc>
#include <osg/AnimationPath>
#include <osg/ApplicationUsage>
#include <osg/ArgumentParser>
#include <osg/Array>
#include <osg/AutoTransform>
#include <osg/Billboard>
#include <osg/BlendColor>
#include <osg/BlendEquation>
#include <osg/BlendFunc>
#include <osg/BoundingBox>
#include <osg/BoundingSphere>
#include <osg/BoundsChecking>
#include <osg/BufferObject>
#include <osg/Camera>
#include <osg/CameraView>
#include <osg/ClearNode>
#include <osg/ClipNode>
#include <osg/ClipPlane>
#include <osg/ClusterCullingCallback>
#include <osg/CollectOccludersVisitor>
#include <osg/ColorMask>
#include <osg/ColorMatrix>
#include <osg/ConvexPlanarOccluder>
#include <osg/ConvexPlanarPolygon>
#include <osg/CoordinateSystemNode>
#include <osg/CopyOp>
#include <osg/CullFace>
#include <osg/CullSettings>
#include <osg/CullStack>
#include <osg/CullingSet>
#include <osg/Depth>
#include <osg/DisplaySettings>
#include <osg/DrawPixels>
#include <osg/Drawable>
#include <osg/Endian>
#include <osg/Export>
#include <osg/Fog>
#include <osg/FragmentProgram>
#include <osg/FrameBufferObject>
#include <osg/FrameStamp>
#include <osg/FrontFace>
#include <osg/GL>
#include <osg/GL2Extensions>
#include <osg/GLExtensions>
#include <osg/GLU>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/GraphicsContext>
#include <osg/GraphicsThread>
#include <osg/Group>
#include <osg/Image>
#include <osg/ImageStream>
#include <osg/LOD>
#include <osg/Light>
#include <osg/LightModel>
#include <osg/LightSource>
#include <osg/LineSegment>
#include <osg/LineStipple>
#include <osg/LineWidth>
#include <osg/LogicOp>
#include <osg/Material>
#include <osg/Math>
#include <osg/Matrix>
#include <osg/MatrixTransform>
#include <osg/Matrixd>
#include <osg/Matrixf>
#include <osg/Multisample>
#include <osg/Node>
#include <osg/NodeCallback>
#include <osg/NodeTrackerCallback>
#include <osg/NodeVisitor>
#include <osg/Notify>
#include <osg/Object>
#include <osg/OccluderNode>
#include <osg/PagedLOD>
#include <osg/Plane>
#include <osg/Point>
#include <osg/PointSprite>
#include <osg/PolygonMode>
#include <osg/PolygonOffset>
#include <osg/PolygonStipple>
#include <osg/Polytope>
#include <osg/PositionAttitudeTransform>
#include <osg/PrimitiveSet>
#include <osg/Program>
#include <osg/Projection>
#include <osg/ProxyNode>
#include <osg/Quat>
#include <osg/Referenced>
#include <osg/Scissor>
#include <osg/Sequence>
#include <osg/ShadeModel>
#include <osg/Shader>
#include <osg/ShadowVolumeOccluder>
#include <osg/Shape>
#include <osg/ShapeDrawable>
#include <osg/State>
#include <osg/StateAttribute>
#include <osg/StateSet>
#include <osg/Stencil>
#include <osg/Switch>
#include <osg/TexEnv>
#include <osg/TexEnvCombine>
#include <osg/TexEnvFilter>
#include <osg/TexGen>
#include <osg/TexGenNode>
#include <osg/TexMat>
#include <osg/Texture>
#include <osg/Texture1D>
#include <osg/Texture2D>
#include <osg/Texture3D>
#include <osg/TextureCubeMap>
#include <osg/TextureRectangle>
#include <osg/Timer>
#include <osg/Transform>
#include <osg/TriangleFunctor>
#include <osg/TriangleIndexFunctor>
#include <osg/Uniform>
#include <osg/Vec2>
#include <osg/Vec2b>
#include <osg/Vec2d>
#include <osg/Vec2f>
#include <osg/Vec2s>
#include <osg/Vec3>
#include <osg/Vec3b>
#include <osg/Vec3d>
#include <osg/Vec3f>
#include <osg/Vec3s>
#include <osg/Vec4>
#include <osg/Vec4b>
#include <osg/Vec4d>
#include <osg/Vec4f>
#include <osg/Vec4s>
#include <osg/Version>
#include <osg/VertexProgram>
#include <osg/Viewport>
#include <osg/buffered_value>
#include <osg/fast_back_stack>
#include <osg/io_utils>
#include <osg/ref_ptr>

#include <osgDB/Archive>
#include <osgDB/DatabasePager>
#include <osgDB/DotOsgWrapper>
#include <osgDB/DynamicLibrary>
#include <osgDB/Export>
#include <osgDB/Field>
#include <osgDB/FieldReader>
#include <osgDB/FieldReaderIterator>
#include <osgDB/FileNameUtils>
#include <osgDB/FileUtils>
#include <osgDB/ImageOptions>
#include <osgDB/Input>
#include <osgDB/Output>
#include <osgDB/ParameterOutput>
#include <osgDB/ReadFile>
#include <osgDB/ReaderWriter>
#include <osgDB/Registry>
#include <osgDB/SharedStateManager>
#include <osgDB/Version>
#include <osgDB/WriteFile>

#include <osgFX/AnisotropicLighting>
#include <osgFX/BumpMapping>
#include <osgFX/Cartoon>
#include <osgFX/Effect>
#include <osgFX/Export>
#include <osgFX/MultiTextureControl>
#include <osgFX/Registry>
#include <osgFX/Scribe>
#include <osgFX/SpecularHighlights>
#include <osgFX/Technique>
#include <osgFX/Validator>

#include <osgGA/AnimationPathManipulator>
#include <osgGA/DriveManipulator>
#include <osgGA/EventQueue>
#include <osgGA/EventVisitor>
#include <osgGA/Export>
#include <osgGA/FlightManipulator>
#include <osgGA/GUIActionAdapter>
#include <osgGA/GUIEventAdapter>
#include <osgGA/GUIEventHandler>
#include <osgGA/KeySwitchMatrixManipulator>
#include <osgGA/MatrixManipulator>
#include <osgGA/NodeTrackerManipulator>
#include <osgGA/StateSetManipulator>
#include <osgGA/TerrainManipulator>
#include <osgGA/TrackballManipulator>
#include <osgGA/UFOManipulator>
#include <osgGA/Version>


#include <osgIntrospection/Attributes>
#include <osgIntrospection/Comparator>
#include <osgIntrospection/ConstructorInfo>
#include <osgIntrospection/Converter>
#include <osgIntrospection/ConverterProxy>
#include <osgIntrospection/CustomAttribute>
#include <osgIntrospection/CustomAttributeProvider>
#include <osgIntrospection/Exceptions>
#include <osgIntrospection/Export>
#include <osgIntrospection/InstanceCreator>
#include <osgIntrospection/ParameterInfo>
#include <osgIntrospection/ReaderWriter>
#include <osgIntrospection/Reflection>
#include <osgIntrospection/Type>
#include <osgIntrospection/TypeNameAliasProxy>
#include <osgIntrospection/Value>
#include <osgIntrospection/variant_cast>

/*
These have been removed b/c they are throwing the following
error in AQAudio:

AQAudio.o:(.rodata._ZTVN16osgIntrospection10MethodInfoE[vtable for 
osgIntrospection::MethodInfo]+0x10): undefined reference to 
`osgIntrospection::MethodInfo::getInheritedProviders(
std::vector<osgIntrospection::CustomAttributeProvider const*, 
std::allocator<osgIntrospection::CustomAttributeProvider const*> >&) const'
collect2: ld returned 1 exit status
make: *** [boeingDemo] Error 1

It would be really good if we could figure out why this error exists for 
future development!!!

#include <osgIntrospection/MethodInfo>
#include <osgIntrospection/PropertyInfo>
#include <osgIntrospection/ReflectionMacros>
#include <osgIntrospection/Reflector>
#include <osgIntrospection/StaticMethodInfo>
#include <osgIntrospection/TypedConstructorInfo>
#include <osgIntrospection/TypedMethodInfo>
#include <osgIntrospection/Utility>
*/

#include <osgParticle/AccelOperator>
#include <osgParticle/AngularAccelOperator>
#include <osgParticle/BoxPlacer>
#include <osgParticle/CenteredPlacer>
#include <osgParticle/ConnectedParticleSystem>
#include <osgParticle/ConstantRateCounter>
#include <osgParticle/Counter>
#include <osgParticle/Emitter>
#include <osgParticle/ExplosionDebrisEffect>
#include <osgParticle/ExplosionEffect>
#include <osgParticle/Export>
#include <osgParticle/FireEffect>
#include <osgParticle/FluidFrictionOperator>
#include <osgParticle/FluidProgram>
#include <osgParticle/ForceOperator>
#include <osgParticle/Interpolator>
#include <osgParticle/LinearInterpolator>
#include <osgParticle/ModularEmitter>
#include <osgParticle/ModularProgram>
#include <osgParticle/MultiSegmentPlacer>
#include <osgParticle/Operator>
#include <osgParticle/Particle>
#include <osgParticle/ParticleEffect>
#include <osgParticle/ParticleProcessor>
#include <osgParticle/ParticleSystem>
#include <osgParticle/ParticleSystemUpdater>
#include <osgParticle/Placer>
#include <osgParticle/PointPlacer>
#include <osgParticle/Program>
#include <osgParticle/RadialShooter>
#include <osgParticle/RandomRateCounter>
#include <osgParticle/SectorPlacer>
#include <osgParticle/SegmentPlacer>
#include <osgParticle/Shooter>
#include <osgParticle/SmokeEffect>
#include <osgParticle/SmokeTrailEffect>
#include <osgParticle/VariableRateCounter>
#include <osgParticle/Version>
#include <osgParticle/range>  

#include <osgViewer/Export>
#include <osgViewer/Version>
#include <osgViewer/Viewer>

#include <osgSim/BlinkSequence>
#include <osgSim/ColorRange>
#include <osgSim/DOFTransform>
#include <osgSim/Export>
#include <osgSim/GeographicLocation>
#include <osgSim/Impostor>
#include <osgSim/ImpostorSprite>
#include <osgSim/InsertImpostorsVisitor>
#include <osgSim/LightPoint>
#include <osgSim/LightPointNode>
#include <osgSim/LightPointSystem>
#include <osgSim/MultiSwitch>
#include <osgSim/OverlayNode>
#include <osgSim/ScalarBar>
#include <osgSim/ScalarsToColors>
#include <osgSim/Sector>
#include <osgSim/SphereSegment>
#include <osgSim/Version>
#include <osgSim/VisibilityGroup>

/* osgTerrain is an optional framework. If you're not using it or don't have it installed,
 * you should comment out this block.
 */
#include <osgTerrain/Export>
#include <osgTerrain/Layer>
#include <osgTerrain/Locator>
#include <osgTerrain/GeometryTechnique>
#include <osgTerrain/Version>

#include <osgText/Export>
#include <osgText/Font>
#include <osgText/String>
#include <osgText/Text>
#include <osgText/Version>

#include <osgUtil/CubeMapGenerator>
#include <osgUtil/CullVisitor>
#include <osgUtil/DelaunayTriangulator>
#include <osgUtil/DisplayRequirementsVisitor>
#include <osgUtil/Export>
#include <osgUtil/GLObjectsVisitor>
#include <osgUtil/HalfWayMapGenerator>
#include <osgUtil/HighlightMapGenerator>
#include <osgUtil/IntersectVisitor>
#include <osgUtil/Optimizer>
#include <osgUtil/PositionalStateContainer>
#include <osgUtil/ReflectionMapGenerator>
#include <osgUtil/RenderBin>
#include <osgUtil/RenderLeaf>
#include <osgUtil/RenderStage>
#include <osgUtil/SceneView>
#include <osgUtil/Simplifier>
#include <osgUtil/SmoothingVisitor>
#include <osgUtil/StateGraph>
#include <osgUtil/Statistics>
#include <osgUtil/TangentSpaceGenerator>
#include <osgUtil/Tessellator>
#include <osgUtil/TransformAttributeFunctor>
#include <osgUtil/TransformCallback>
#include <osgUtil/TriStripVisitor>
#include <osgUtil/UpdateVisitor>
#include <osgUtil/Version>

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

// Amoniq includes
#include <aq/String>
#include <aq/KVReflector>

#endif

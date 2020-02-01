








































#ifndef TINY_GLTF_H_
#define TINY_GLTF_H_

#define TINYGLTF_NO_STB_IMAGE_WRITE



#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <array>
#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include "memory/String.h"
using namespace ptl::string_literals;
#include "memory/Containers.h"
#include "memory/RapidJsonAllocator.h"
#include <fstream>

#ifdef __ANDROID__
#ifdef TINYGLTF_ANDROID_LOAD_FROM_ASSETS
#include <android/asset_manager.h>
#endif
#endif





#define TINYGLTF_NOEXCEPTION
#define DEFINE_DEFAULT_RAPID_JSON_TYPE
#if defined(DEFINE_DEFAULT_RAPID_JSON_TYPE)

namespace json {
	using BaseMemoryAllocator = rapidjson::CrtAllocator;
using MemoryAllocator = rapidjson::MemoryPoolAllocator<BaseMemoryAllocator>;
using StackAllocator = BaseMemoryAllocator;
using Doc = rapidjson::GenericDocument<rapidjson::UTF8<>, MemoryAllocator,
                                       StackAllocator>;

using Value = Doc::ValueType;
using const_iterator = Value::ConstMemberIterator;
using iterator = Value::MemberIterator;
}  
#endif

namespace tinygltf {

#define TINYGLTF_MODE_POINTS (0)
#define TINYGLTF_MODE_LINE (1)
#define TINYGLTF_MODE_LINE_LOOP (2)
#define TINYGLTF_MODE_LINE_STRIP (3)
#define TINYGLTF_MODE_TRIANGLES (4)
#define TINYGLTF_MODE_TRIANGLE_STRIP (5)
#define TINYGLTF_MODE_TRIANGLE_FAN (6)

#define TINYGLTF_COMPONENT_TYPE_BYTE (5120)
#define TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE (5121)
#define TINYGLTF_COMPONENT_TYPE_SHORT (5122)
#define TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT (5123)
#define TINYGLTF_COMPONENT_TYPE_INT (5124)
#define TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT (5125)
#define TINYGLTF_COMPONENT_TYPE_FLOAT (5126)
#define TINYGLTF_COMPONENT_TYPE_DOUBLE (5130)

#define TINYGLTF_TEXTURE_FILTER_NEAREST (9728)
#define TINYGLTF_TEXTURE_FILTER_LINEAR (9729)
#define TINYGLTF_TEXTURE_FILTER_NEAREST_MIPMAP_NEAREST (9984)
#define TINYGLTF_TEXTURE_FILTER_LINEAR_MIPMAP_NEAREST (9985)
#define TINYGLTF_TEXTURE_FILTER_NEAREST_MIPMAP_LINEAR (9986)
#define TINYGLTF_TEXTURE_FILTER_LINEAR_MIPMAP_LINEAR (9987)

#define TINYGLTF_TEXTURE_WRAP_REPEAT (10497)
#define TINYGLTF_TEXTURE_WRAP_CLAMP_TO_EDGE (33071)
#define TINYGLTF_TEXTURE_WRAP_MIRRORED_REPEAT (33648)


#define TINYGLTF_PARAMETER_TYPE_BYTE (5120)
#define TINYGLTF_PARAMETER_TYPE_UNSIGNED_BYTE (5121)
#define TINYGLTF_PARAMETER_TYPE_SHORT (5122)
#define TINYGLTF_PARAMETER_TYPE_UNSIGNED_SHORT (5123)
#define TINYGLTF_PARAMETER_TYPE_INT (5124)
#define TINYGLTF_PARAMETER_TYPE_UNSIGNED_INT (5125)
#define TINYGLTF_PARAMETER_TYPE_FLOAT (5126)

#define TINYGLTF_PARAMETER_TYPE_FLOAT_VEC2 (35664)
#define TINYGLTF_PARAMETER_TYPE_FLOAT_VEC3 (35665)
#define TINYGLTF_PARAMETER_TYPE_FLOAT_VEC4 (35666)

#define TINYGLTF_PARAMETER_TYPE_INT_VEC2 (35667)
#define TINYGLTF_PARAMETER_TYPE_INT_VEC3 (35668)
#define TINYGLTF_PARAMETER_TYPE_INT_VEC4 (35669)

#define TINYGLTF_PARAMETER_TYPE_BOOL (35670)
#define TINYGLTF_PARAMETER_TYPE_BOOL_VEC2 (35671)
#define TINYGLTF_PARAMETER_TYPE_BOOL_VEC3 (35672)
#define TINYGLTF_PARAMETER_TYPE_BOOL_VEC4 (35673)

#define TINYGLTF_PARAMETER_TYPE_FLOAT_MAT2 (35674)
#define TINYGLTF_PARAMETER_TYPE_FLOAT_MAT3 (35675)
#define TINYGLTF_PARAMETER_TYPE_FLOAT_MAT4 (35676)

#define TINYGLTF_PARAMETER_TYPE_SAMPLER_2D (35678)



#define TINYGLTF_TYPE_VEC2 (2)
#define TINYGLTF_TYPE_VEC3 (3)
#define TINYGLTF_TYPE_VEC4 (4)
#define TINYGLTF_TYPE_MAT2 (32 + 2)
#define TINYGLTF_TYPE_MAT3 (32 + 3)
#define TINYGLTF_TYPE_MAT4 (32 + 4)
#define TINYGLTF_TYPE_SCALAR (64 + 1)
#define TINYGLTF_TYPE_VECTOR (64 + 4)
#define TINYGLTF_TYPE_MATRIX (64 + 16)

#define TINYGLTF_IMAGE_FORMAT_JPEG (0)
#define TINYGLTF_IMAGE_FORMAT_PNG (1)
#define TINYGLTF_IMAGE_FORMAT_BMP (2)
#define TINYGLTF_IMAGE_FORMAT_GIF (3)

#define TINYGLTF_TEXTURE_FORMAT_ALPHA (6406)
#define TINYGLTF_TEXTURE_FORMAT_RGB (6407)
#define TINYGLTF_TEXTURE_FORMAT_RGBA (6408)
#define TINYGLTF_TEXTURE_FORMAT_LUMINANCE (6409)
#define TINYGLTF_TEXTURE_FORMAT_LUMINANCE_ALPHA (6410)

#define TINYGLTF_TEXTURE_TARGET_TEXTURE2D (3553)
#define TINYGLTF_TEXTURE_TYPE_UNSIGNED_BYTE (5121)

#define TINYGLTF_TARGET_ARRAY_BUFFER (34962)
#define TINYGLTF_TARGET_ELEMENT_ARRAY_BUFFER (34963)

#define TINYGLTF_SHADER_TYPE_VERTEX_SHADER (35633)
#define TINYGLTF_SHADER_TYPE_FRAGMENT_SHADER (35632)

#define TINYGLTF_DOUBLE_EPS (1.e-12)
#define TINYGLTF_DOUBLE_EQUAL(a, b) (std::fabs((b) - (a)) < TINYGLTF_DOUBLE_EPS)

#ifdef __ANDROID__
#ifdef TINYGLTF_ANDROID_LOAD_FROM_ASSETS
AAssetManager *asset_manager = nullptr;
#endif
#endif

typedef enum {
  NULL_TYPE = 0,
  NUMBER_TYPE = 1,
  INT_TYPE = 2,
  BOOL_TYPE = 3,
  STRING_TYPE = 4,
  ARRAY_TYPE = 5,
  BINARY_TYPE = 6,
  OBJECT_TYPE = 7
} Type;

static inline int32_t GetComponentSizeInBytes(uint32_t componentType) {
  if (componentType == TINYGLTF_COMPONENT_TYPE_BYTE) {
    return 1;
  } else if (componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE) {
    return 1;
  } else if (componentType == TINYGLTF_COMPONENT_TYPE_SHORT) {
    return 2;
  } else if (componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT) {
    return 2;
  } else if (componentType == TINYGLTF_COMPONENT_TYPE_INT) {
    return 4;
  } else if (componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT) {
    return 4;
  } else if (componentType == TINYGLTF_COMPONENT_TYPE_FLOAT) {
    return 4;
  } else if (componentType == TINYGLTF_COMPONENT_TYPE_DOUBLE) {
    return 8;
  } else {
    
    return -1;
  }
}

static inline int32_t GetTypeSizeInBytes(uint32_t ty) {
  if (ty == TINYGLTF_TYPE_SCALAR) {
    return 1;
  } else if (ty == TINYGLTF_TYPE_VEC2) {
    return 2;
  } else if (ty == TINYGLTF_TYPE_VEC3) {
    return 3;
  } else if (ty == TINYGLTF_TYPE_VEC4) {
    return 4;
  } else if (ty == TINYGLTF_TYPE_MAT2) {
    return 4;
  } else if (ty == TINYGLTF_TYPE_MAT3) {
    return 9;
  } else if (ty == TINYGLTF_TYPE_MAT4) {
    return 16;
  } else {
    
    return -1;
  }
}

bool IsDataURI(const ptl::string &in);
bool DecodeDataURI(ptl::vector<unsigned char> *out, ptl::string &mime_type,
                   const ptl::string &in, size_t reqBytes, bool checkSize);

#ifdef __clang__
#pragma clang diagnostic push


#pragma clang diagnostic ignored "-Wexit-time-destructors"
#pragma clang diagnostic ignored "-Wpadded"
#endif


class Value {
 public:
  typedef ptl::vector<Value> Array;
  typedef ptl::map<ptl::string, Value> Object;

  Value() : type_(NULL_TYPE) {}

  explicit Value(bool b) : type_(BOOL_TYPE) { boolean_value_ = b; }
  explicit Value(int i) : type_(INT_TYPE) { int_value_ = i; }
  explicit Value(double n) : type_(NUMBER_TYPE) { number_value_ = n; }
  explicit Value(const ptl::string &s) : type_(STRING_TYPE) {
    string_value_ = s;
  }
  explicit Value(const unsigned char *p, size_t n) : type_(BINARY_TYPE) {
    binary_value_.resize(n);
    memcpy(binary_value_.data(), p, n);
  }
  explicit Value(const Array &a) : type_(ARRAY_TYPE) {
    array_value_ = Array(a);
  }
  explicit Value(const Object &o) : type_(OBJECT_TYPE) {
    object_value_ = Object(o);
  }

  char Type() const { return static_cast<const char>(type_); }

  bool IsBool() const { return (type_ == BOOL_TYPE); }

  bool IsInt() const { return (type_ == INT_TYPE); }

  bool IsNumber() const { return (type_ == NUMBER_TYPE); }

  bool IsString() const { return (type_ == STRING_TYPE); }

  bool IsBinary() const { return (type_ == BINARY_TYPE); }

  bool IsArray() const { return (type_ == ARRAY_TYPE); }

  bool IsObject() const { return (type_ == OBJECT_TYPE); }

  
  template <typename T>
  const T &Get() const;
  template <typename T>
  T &Get();

  
  const Value &Get(int idx) const {
    static Value null_value;
    assert(IsArray());
    assert(idx >= 0);
    return (static_cast<size_t>(idx) < array_value_.size())
               ? array_value_[static_cast<size_t>(idx)]
               : null_value;
  }

  
  const Value &Get(const ptl::string &key) const {
    static Value null_value;
    assert(IsObject());
    Object::const_iterator it = object_value_.find(key);
    return (it != object_value_.end()) ? it->second : null_value;
  }

  size_t ArrayLen() const {
    if (!IsArray()) return 0;
    return array_value_.size();
  }

  
  bool Has(const ptl::string &key) const {
    if (!IsObject()) return false;
    Object::const_iterator it = object_value_.find(key);
    return (it != object_value_.end()) ? true : false;
  }

  
  ptl::vector<ptl::string> Keys() const {
    ptl::vector<ptl::string> keys;
    if (!IsObject()) return keys;  

    for (Object::const_iterator it = object_value_.begin();
         it != object_value_.end(); ++it) {
      keys.push_back(it->first);
    }

    return keys;
  }

  size_t Size() const { return (IsArray() ? ArrayLen() : Keys().size()); }

  bool operator==(const tinygltf::Value &other) const;

 protected:
  int type_;

  int int_value_;
  double number_value_;
  ptl::string string_value_;
  ptl::vector<unsigned char> binary_value_;
  Array array_value_;
  Object object_value_;
  bool boolean_value_;
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#define TINYGLTF_VALUE_GET(ctype, var)            \
  template <>                                     \
  inline const ctype &Value::Get<ctype>() const { \
    return var;                                   \
  }                                               \
  template <>                                     \
  inline ctype &Value::Get<ctype>() {             \
    return var;                                   \
  }
TINYGLTF_VALUE_GET(bool, boolean_value_)
TINYGLTF_VALUE_GET(double, number_value_)
TINYGLTF_VALUE_GET(int, int_value_)
TINYGLTF_VALUE_GET(ptl::string, string_value_)
TINYGLTF_VALUE_GET(ptl::vector<unsigned char>, binary_value_)
TINYGLTF_VALUE_GET(Value::Array, array_value_)
TINYGLTF_VALUE_GET(Value::Object, object_value_)
#undef TINYGLTF_VALUE_GET

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wc++98-compat"
#pragma clang diagnostic ignored "-Wpadded"
#endif


using ColorValue = std::array<double, 4>;

struct Parameter {
  bool bool_value = false;
  bool has_number_value = false;
  ptl::string string_value;
  ptl::vector<double> number_array;
  ptl::map<ptl::string, double> json_double_value;
  double number_value = 0.0;
  
  
  
  

  
  
  
  int TextureIndex() const {
    const auto it = json_double_value.find("index");
    if (it != std::end(json_double_value)) {
      return int(it->second);
    }
    return -1;
  }

  
  
  
  int TextureTexCoord() const {
    const auto it = json_double_value.find("texCoord");
    if (it != std::end(json_double_value)) {
      return int(it->second);
    }
    return 0;
  }

  
  
  
  double Factor() const { return number_value; }

  
  
  
  ColorValue ColorFactor() const {
    return {
        {
         number_array[0], number_array[1], number_array[2],
         (number_array.size() > 3 ? number_array[3] : 1.0)}};
  }

  bool operator==(const Parameter &) const;
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpadded"
#endif

typedef ptl::map<ptl::string, Parameter> ParameterMap;
typedef ptl::map<ptl::string, Value> ExtensionMap;

struct AnimationChannel {
  int sampler;              
  int target_node;          
  ptl::string target_path;  
                            
  Value extras;

  AnimationChannel() : sampler(-1), target_node(-1) {}
  bool operator==(const AnimationChannel &) const;
};

struct AnimationSampler {
  int input;                  
  int output;                 
  ptl::string interpolation;  
                              
  Value extras;

  AnimationSampler() : input(-1), output(-1), interpolation("LINEAR") {}
  bool operator==(const AnimationSampler &) const;
};

struct Animation {
  ptl::string name;
  ptl::vector<AnimationChannel> channels;
  ptl::vector<AnimationSampler> samplers;
  Value extras;

  bool operator==(const Animation &) const;
};

struct Skin {
  ptl::string name;
  int inverseBindMatrices;  
  int skeleton;             
  ptl::vector<int> joints;  

  Skin() {
    inverseBindMatrices = -1;
    skeleton = -1;
  }
  bool operator==(const Skin &) const;
};

struct Sampler {
  ptl::string name;
  int minFilter;  
                  
                  
  int magFilter;  
  int wrapS;      
                  
  int wrapT;      
                  
  int wrapR;      
  Value extras;

  Sampler()
      : minFilter(TINYGLTF_TEXTURE_FILTER_LINEAR_MIPMAP_LINEAR),
        magFilter(TINYGLTF_TEXTURE_FILTER_LINEAR),
        wrapS(TINYGLTF_TEXTURE_WRAP_REPEAT),
        wrapT(TINYGLTF_TEXTURE_WRAP_REPEAT),
        wrapR(TINYGLTF_TEXTURE_WRAP_REPEAT) {}
  bool operator==(const Sampler &) const;
};

struct Image {
  ptl::string name;
  int width;
  int height;
  int component;
  int bits;        
  int pixel_type;  
				   
  ptl::vector<unsigned char> image;
  int bufferView;        
  ptl::string mimeType;  
                         
  ptl::string uri;       
  Value extras;
  ExtensionMap extensions;

  
  
  
  
  
  
  bool as_is;

  Image() : as_is(false) {
    bufferView = -1;
    width = -1;
    height = -1;
    component = -1;
  }
  bool operator==(const Image &) const;
};

struct Texture {
  ptl::string name;

  int sampler;
  int source;
  Value extras;
  ExtensionMap extensions;

  Texture() : sampler(-1), source(-1) {}
  bool operator==(const Texture &) const;
};




struct Material {
  ptl::string name;

  ParameterMap values;            
  ParameterMap additionalValues;  

  ExtensionMap extensions;
  Value extras;

  bool operator==(const Material &) const;
};

struct BufferView {
  ptl::string name;
  int buffer;         
  size_t byteOffset;  
  size_t byteLength;  
  size_t byteStride;  
                      
  int target;         
  Value extras;
  bool dracoDecoded;  

  BufferView() : byteOffset(0), byteStride(0), dracoDecoded(false) {}
  bool operator==(const BufferView &) const;
};

struct Accessor {
  int bufferView;  
                   
  ptl::string name;
  size_t byteOffset;
  bool normalized;    
  int componentType;  
  size_t count;       
  int type;           
  Value extras;

  ptl::vector<double> minValues;  
  ptl::vector<double> maxValues;  

  struct {
    int count;
    bool isSparse;
    struct {
      int byteOffset;
      int bufferView;
      int componentType;  
    } indices;
    struct {
      int bufferView;
      int byteOffset;
    } values;
  } sparse;

  
  
  
  
  int ByteStride(const BufferView &bufferViewObject) const {
    if (bufferViewObject.byteStride == 0) {
      
      int componentSizeInBytes =
          GetComponentSizeInBytes(static_cast<uint32_t>(componentType));
      if (componentSizeInBytes <= 0) {
        return -1;
      }

      int typeSizeInBytes = GetTypeSizeInBytes(static_cast<uint32_t>(type));
      if (typeSizeInBytes <= 0) {
        return -1;
      }

      return componentSizeInBytes * typeSizeInBytes;
    } else {
      
      
      int componentSizeInBytes =
          GetComponentSizeInBytes(static_cast<uint32_t>(componentType));
      if (componentSizeInBytes <= 0) {
        return -1;
      }

      if ((bufferViewObject.byteStride % uint32_t(componentSizeInBytes)) != 0) {
        return -1;
      }
      return static_cast<int>(bufferViewObject.byteStride);
    }
  }

  Accessor() {
    bufferView = -1;
    sparse.isSparse = false;
  }
  bool operator==(const tinygltf::Accessor &) const;
};

struct PerspectiveCamera {
  double aspectRatio;  
  double yfov;         
  double zfar;         
  double znear;        

  PerspectiveCamera()
      : aspectRatio(0.0),
        yfov(0.0),
        zfar(0.0)  
        ,
        znear(0.0) {}
  bool operator==(const PerspectiveCamera &) const;

  ExtensionMap extensions;
  Value extras;
};

struct OrthographicCamera {
  double xmag;   
  double ymag;   
  double zfar;   
  double znear;  

  OrthographicCamera() : xmag(0.0), ymag(0.0), zfar(0.0), znear(0.0) {}
  bool operator==(const OrthographicCamera &) const;

  ExtensionMap extensions;
  Value extras;
};

struct Camera {
  ptl::string type;  
  ptl::string name;

  PerspectiveCamera perspective;
  OrthographicCamera orthographic;

  Camera() {}
  bool operator==(const Camera &) const;

  ExtensionMap extensions;
  Value extras;
};

struct Primitive {
  ptl::map<ptl::string, int> attributes;  
                                          
                                          
                                          
  int material;  
                 
  int indices;   
  int mode;      
  ptl::vector<ptl::map<ptl::string, int> > targets;  
  
  
  
  ExtensionMap extensions;
  Value extras;

  Primitive() {
    material = -1;
    indices = -1;
  }
  bool operator==(const Primitive &) const;
};

struct Mesh {
  ptl::string name;
  ptl::vector<Primitive> primitives;
  ptl::vector<double> weights;  
  ptl::vector<ptl::map<ptl::string, int> > targets;
  ExtensionMap extensions;
  Value extras;

  bool operator==(const Mesh &) const;
};

class Node {
 public:
  Node() : camera(-1), skin(-1), mesh(-1) {}

  Node(const Node &rhs) {
    camera = rhs.camera;

    name = rhs.name;
    skin = rhs.skin;
    mesh = rhs.mesh;
    children = rhs.children;
    rotation = rhs.rotation;
    scale = rhs.scale;
    translation = rhs.translation;
    matrix = rhs.matrix;
    weights = rhs.weights;

    extensions = rhs.extensions;
    extras = rhs.extras;
  }
  ~Node() {}
  bool operator==(const Node &) const;

  int camera;  

  ptl::string name;
  int skin;
  int mesh;
  ptl::vector<int> children;
  ptl::vector<double> rotation;     
  ptl::vector<double> scale;        
  ptl::vector<double> translation;  
  ptl::vector<double> matrix;       
  ptl::vector<double> weights;  

  ExtensionMap extensions;
  Value extras;
};

struct Buffer {
  ptl::string name;
  ptl::vector<unsigned char> data;
  ptl::string
      uri;  
  Value extras;

  bool operator==(const Buffer &) const;
};

struct Asset {
  ptl::string version;  
  ptl::string generator;
  ptl::string minVersion;
  ptl::string copyright;
  ExtensionMap extensions;
  Value extras;

  bool operator==(const Asset &) const;
};

struct Scene {
  ptl::string name;
  ptl::vector<int> nodes;

  ExtensionMap extensions;
  Value extras;

  bool operator==(const Scene &) const;
};

struct Light {
  ptl::string name;
  ptl::vector<double> color;
  ptl::string type;
  double intensity;
  double distance;
  double innerConeAngle;
  double outerConeAngle;
  bool operator==(const Light &) const;
};

class Model {
 public:
  Model() {}
  ~Model() {}
  bool operator==(const Model &) const;

  ptl::vector<Accessor> accessors;
  ptl::vector<Animation> animations;
  ptl::vector<Buffer> buffers;
  ptl::vector<BufferView> bufferViews;
  ptl::vector<Material> materials;
  ptl::vector<Mesh> meshes;
  ptl::vector<Node> nodes;
  ptl::vector<Texture> textures;
  ptl::vector<Image> images;
  ptl::vector<Skin> skins;
  ptl::vector<Sampler> samplers;
  ptl::vector<Camera> cameras;
  ptl::vector<Scene> scenes;
  ptl::vector<Light> lights;
  ExtensionMap extensions;

  int defaultScene;
  ptl::vector<ptl::string> extensionsUsed;
  ptl::vector<ptl::string> extensionsRequired;

  Asset asset;

  Value extras;
};

enum SectionCheck {
  NO_REQUIRE = 0x00,
  REQUIRE_SCENE = 0x01,
  REQUIRE_SCENES = 0x02,
  REQUIRE_NODES = 0x04,
  REQUIRE_ACCESSORS = 0x08,
  REQUIRE_BUFFERS = 0x10,
  REQUIRE_BUFFER_VIEWS = 0x20,
  REQUIRE_ALL = 0x3f
};




typedef bool (*LoadImageDataFunction)(Image *, const int, ptl::string *,
                                      ptl::string *, int, int,
                                      const unsigned char *, int, void *);




typedef bool (*WriteImageDataFunction)(const ptl::string *, const ptl::string *,
                                       Image *, bool, void *);

#ifndef TINYGLTF_NO_STB_IMAGE

bool LoadImageData(Image *image, const int image_idx, ptl::string *err,
                   ptl::string *warn, int req_width, int req_height,
                   const unsigned char *bytes, int size, void *);
#endif

#ifndef TINYGLTF_NO_STB_IMAGE_WRITE

bool WriteImageData(const ptl::string *basepath, const ptl::string *filename,
                    Image *image, bool embedImages, void *);
#endif




typedef bool (*FileExistsFunction)(const ptl::string &abs_filename, void *);




typedef ptl::string (*ExpandFilePathFunction)(const ptl::string &, void *);




typedef bool (*ReadWholeFileFunction)(ptl::vector<unsigned char> *,
                                      ptl::string *, const ptl::string &,
                                      void *);




typedef bool (*WriteWholeFileFunction)(ptl::string *, const ptl::string &,
                                       const ptl::vector<unsigned char> &,
                                       void *);





struct FsCallbacks {
  FileExistsFunction FileExists;
  ExpandFilePathFunction ExpandFilePath;
  ReadWholeFileFunction ReadWholeFile;
  WriteWholeFileFunction WriteWholeFile;

  void *user_data;  
};

#ifndef TINYGLTF_NO_FS


bool FileExists(const ptl::string &abs_filename, void *);

ptl::string ExpandFilePath(const ptl::string &filepath, void *);

bool ReadWholeFile(ptl::vector<unsigned char> *out, ptl::string *err,
                   const ptl::string &filepath, void *);

bool WriteWholeFile(ptl::string *err, const ptl::string &filepath,
                    const ptl::vector<unsigned char> &contents, void *);
#endif

class TinyGLTF {
 public:
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wc++98-compat"
#endif

  TinyGLTF() : bin_data_(nullptr), bin_size_(0), is_binary_(false) {}

#ifdef __clang__
#pragma clang diagnostic pop
#endif

  ~TinyGLTF() {}

  
  
  
  
  
  bool LoadASCIIFromFile(Model *model, ptl::string *err, ptl::string *warn,
                         const ptl::string &filename,
                         unsigned int check_sections = REQUIRE_ALL);

  
  
  
  
  
  
  bool LoadASCIIFromString(Model *model, ptl::string *err, ptl::string *warn,
                           const char *str, const unsigned int length,
                           const ptl::string &base_dir,
                           unsigned int check_sections = REQUIRE_ALL);

  
  
  
  
  
  bool LoadBinaryFromFile(Model *model, ptl::string *err, ptl::string *warn,
                          const ptl::string &filename,
                          unsigned int check_sections = REQUIRE_ALL);

  
  
  
  
  
  
  bool LoadBinaryFromMemory(Model *model, ptl::string *err, ptl::string *warn,
                            const unsigned char *bytes,
                            const unsigned int length,
                            const ptl::string &base_dir = "",
                            unsigned int check_sections = REQUIRE_ALL);

  
  
  
  bool WriteGltfSceneToFile(Model *model, const ptl::string &filename,
                            bool embedImages, bool embedBuffers,
                            bool prettyPrint, bool writeBinary);

  
  
  
  void SetImageLoader(LoadImageDataFunction LoadImageData, void *user_data);

  
  
  
  void SetImageWriter(WriteImageDataFunction WriteImageData, void *user_data);

  
  
  
  void SetFsCallbacks(FsCallbacks callbacks);

 private:
  
  
  
  
  
  
  bool LoadFromString(Model *model, ptl::string *err, ptl::string *warn,
                      const char *str, const unsigned int length,
                      const ptl::string &base_dir, unsigned int check_sections);

  const unsigned char *bin_data_;
  size_t bin_size_;
  bool is_binary_;

  FsCallbacks fs = {
#ifndef TINYGLTF_NO_FS
      &tinygltf::FileExists, &tinygltf::ExpandFilePath,
      &tinygltf::ReadWholeFile, &tinygltf::WriteWholeFile,

      nullptr  
#else
      nullptr, nullptr, nullptr, nullptr,

      nullptr  
#endif
  };

  LoadImageDataFunction LoadImageData =
#ifndef TINYGLTF_NO_STB_IMAGE
      &tinygltf::LoadImageData;
#else
      nullptr;
#endif
  void *load_image_user_data_ = reinterpret_cast<void *>(&fs);

  WriteImageDataFunction WriteImageData =
#ifndef TINYGLTF_NO_STB_IMAGE_WRITE
      &tinygltf::WriteImageData;
#else
      nullptr;
#endif
  void *write_image_user_data_ = reinterpret_cast<void *>(&fs);
};

#ifdef __clang__
#pragma clang diagnostic pop  
#endif

}  

#endif  

#if defined(TINYGLTF_IMPLEMENTATION) || defined(__INTELLISENSE__) 
#include <algorithm>

#ifndef TINYGLTF_NO_FS
#include <fstream>
#endif
#include <sstream>

#ifdef __clang__

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wfloat-equal"
#pragma clang diagnostic ignored "-Wexit-time-destructors"
#pragma clang diagnostic ignored "-Wconversion"
#pragma clang diagnostic ignored "-Wold-style-cast"
#pragma clang diagnostic ignored "-Wglobal-constructors"
#pragma clang diagnostic ignored "-Wreserved-id-macro"
#pragma clang diagnostic ignored "-Wdisabled-macro-expansion"
#pragma clang diagnostic ignored "-Wpadded"
#pragma clang diagnostic ignored "-Wc++98-compat"
#pragma clang diagnostic ignored "-Wc++98-compat-pedantic"
#pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
#pragma clang diagnostic ignored "-Wswitch-enum"
#pragma clang diagnostic ignored "-Wimplicit-fallthrough"
#pragma clang diagnostic ignored "-Wweak-vtables"
#pragma clang diagnostic ignored "-Wcovered-switch-default"
#if __has_warning("-Wdouble-promotion")
#pragma clang diagnostic ignored "-Wdouble-promotion"
#endif
#if __has_warning("-Wcomma")
#pragma clang diagnostic ignored "-Wcomma"
#endif
#if __has_warning("-Wzero-as-null-pointer-constant")
#pragma clang diagnostic ignored "-Wzero-as-null-pointer-constant"
#endif
#if __has_warning("-Wcast-qual")
#pragma clang diagnostic ignored "-Wcast-qual"
#endif
#if __has_warning("-Wmissing-variable-declarations")
#pragma clang diagnostic ignored "-Wmissing-variable-declarations"
#endif
#if __has_warning("-Wmissing-prototypes")
#pragma clang diagnostic ignored "-Wmissing-prototypes"
#endif
#if __has_warning("-Wcast-align")
#pragma clang diagnostic ignored "-Wcast-align"
#endif
#if __has_warning("-Wnewline-eof")
#pragma clang diagnostic ignored "-Wnewline-eof"
#endif
#if __has_warning("-Wunused-parameter")
#pragma clang diagnostic ignored "-Wunused-parameter"
#endif
#if __has_warning("-Wmismatched-tags")
#pragma clang diagnostic ignored "-Wmismatched-tags"
#endif
#endif

#ifndef TINYGLTF_NO_INCLUDE_JSON
#include "rapidjson/rapidjson.h"

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wtype-limits"
#endif  
#endif

#ifdef TINYGLTF_ENABLE_DRACO
#include "draco/compression/decode.h"
#include "draco/core/decoder_buffer.h"
#endif

#ifndef TINYGLTF_NO_STB_IMAGE
#ifndef TINYGLTF_NO_INCLUDE_STB_IMAGE
#include "stb_image.h"
#endif
#endif

#ifndef TINYGLTF_NO_STB_IMAGE_WRITE
#ifndef TINYGLTF_NO_INCLUDE_STB_IMAGE_WRITE
#include "stb_image_write.h"
#endif
#endif

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

#ifdef _WIN32




#ifndef NOMINMAX
#define TINYGLTF_INTERNAL_NOMINMAX
#define NOMINMAX
#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#define TINYGLTF_INTERNAL_WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>  

#ifdef TINYGLTF_INTERNAL_WIN32_LEAN_AND_MEAN
#undef WIN32_LEAN_AND_MEAN
#endif

#if defined(TINYGLTF_INTERNAL_NOMINMAX)
#undef NOMINMAX
#endif

#endif

#if defined(__sparcv9)

#else
#if (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__) || MINIZ_X86_OR_X64_CPU
#define TINYGLTF_LITTLE_ENDIAN 1
#endif
#endif

#ifdef __APPLE__
#include "TargetConditionals.h"
#endif

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wc++98-compat"
#endif

namespace tinygltf {


static bool Equals(const tinygltf::Value &one, const tinygltf::Value &other) {
  if (one.Type() != other.Type()) return false;

  switch (one.Type()) {
    case NULL_TYPE:
      return true;
    case BOOL_TYPE:
      return one.Get<bool>() == other.Get<bool>();
    case NUMBER_TYPE:
      return TINYGLTF_DOUBLE_EQUAL(one.Get<double>(), other.Get<double>());
    case INT_TYPE:
      return one.Get<int>() == other.Get<int>();
    case OBJECT_TYPE: {
      auto oneObj = one.Get<tinygltf::Value::Object>();
      auto otherObj = other.Get<tinygltf::Value::Object>();
      if (oneObj.size() != otherObj.size()) return false;
      for (auto &it : oneObj) {
        auto otherIt = otherObj.find(it.first);
        if (otherIt == otherObj.end()) return false;

        if (!Equals(it.second, otherIt->second)) return false;
      }
      return true;
    }
    case ARRAY_TYPE: {
      if (one.Size() != other.Size()) return false;
      for (int i = 0; i < int(one.Size()); ++i)
        if (Equals(one.Get(i), other.Get(i))) return false;
      return true;
    }
    case STRING_TYPE:
      return one.Get<ptl::string>() == other.Get<ptl::string>();
    case BINARY_TYPE:
      return one.Get<ptl::vector<unsigned char> >() ==
             other.Get<ptl::vector<unsigned char> >();
    default: {
      
      return false;
    }
  }
}


static bool Equals(const ptl::vector<double> &one,
                   const ptl::vector<double> &other) {
  if (one.size() != other.size()) return false;
  for (int i = 0; i < int(one.size()); ++i) {
    if (!TINYGLTF_DOUBLE_EQUAL(one[size_t(i)], other[size_t(i)])) return false;
  }
  return true;
}

bool Accessor::operator==(const Accessor &other) const {
  return this->bufferView == other.bufferView &&
         this->byteOffset == other.byteOffset &&
         this->componentType == other.componentType &&
         this->count == other.count && this->extras == other.extras &&
         Equals(this->maxValues, other.maxValues) &&
         Equals(this->minValues, other.minValues) && this->name == other.name &&
         this->normalized == other.normalized && this->type == other.type;
}
bool Animation::operator==(const Animation &other) const {
  return this->channels == other.channels && this->extras == other.extras &&
         this->name == other.name && this->samplers == other.samplers;
}
bool AnimationChannel::operator==(const AnimationChannel &other) const {
  return this->extras == other.extras &&
         this->target_node == other.target_node &&
         this->target_path == other.target_path &&
         this->sampler == other.sampler;
}
bool AnimationSampler::operator==(const AnimationSampler &other) const {
  return this->extras == other.extras && this->input == other.input &&
         this->interpolation == other.interpolation &&
         this->output == other.output;
}
bool Asset::operator==(const Asset &other) const {
  return this->copyright == other.copyright &&
         this->extensions == other.extensions && this->extras == other.extras &&
         this->generator == other.generator &&
         this->minVersion == other.minVersion && this->version == other.version;
}
bool Buffer::operator==(const Buffer &other) const {
  return this->data == other.data && this->extras == other.extras &&
         this->name == other.name && this->uri == other.uri;
}
bool BufferView::operator==(const BufferView &other) const {
  return this->buffer == other.buffer && this->byteLength == other.byteLength &&
         this->byteOffset == other.byteOffset &&
         this->byteStride == other.byteStride && this->name == other.name &&
         this->target == other.target && this->extras == other.extras &&
         this->dracoDecoded == other.dracoDecoded;
}
bool Camera::operator==(const Camera &other) const {
  return this->name == other.name && this->extensions == other.extensions &&
         this->extras == other.extras &&
         this->orthographic == other.orthographic &&
         this->perspective == other.perspective && this->type == other.type;
}
bool Image::operator==(const Image &other) const {
  return this->bufferView == other.bufferView &&
         this->component == other.component && this->extras == other.extras &&
         this->height == other.height && this->image == other.image &&
         this->mimeType == other.mimeType && this->name == other.name &&
         this->uri == other.uri && this->width == other.width;
}
bool Light::operator==(const Light &other) const {
  return Equals(this->color, other.color) && this->name == other.name &&
         this->type == other.type;
}
bool Material::operator==(const Material &other) const {
  return this->additionalValues == other.additionalValues &&
         this->extensions == other.extensions && this->extras == other.extras &&
         this->name == other.name && this->values == other.values;
}
bool Mesh::operator==(const Mesh &other) const {
  return this->extensions == other.extensions && this->extras == other.extras &&
         this->name == other.name && this->primitives == other.primitives &&
         this->targets == other.targets && Equals(this->weights, other.weights);
}
bool Model::operator==(const Model &other) const {
  return this->accessors == other.accessors &&
         this->animations == other.animations && this->asset == other.asset &&
         this->buffers == other.buffers &&
         this->bufferViews == other.bufferViews &&
         this->cameras == other.cameras &&
         this->defaultScene == other.defaultScene &&
         this->extensions == other.extensions &&
         this->extensionsRequired == other.extensionsRequired &&
         this->extensionsUsed == other.extensionsUsed &&
         this->extras == other.extras && this->images == other.images &&
         this->lights == other.lights && this->materials == other.materials &&
         this->meshes == other.meshes && this->nodes == other.nodes &&
         this->samplers == other.samplers && this->scenes == other.scenes &&
         this->skins == other.skins && this->textures == other.textures;
}
bool Node::operator==(const Node &other) const {
  return this->camera == other.camera && this->children == other.children &&
         this->extensions == other.extensions && this->extras == other.extras &&
         Equals(this->matrix, other.matrix) && this->mesh == other.mesh &&
         this->name == other.name && Equals(this->rotation, other.rotation) &&
         Equals(this->scale, other.scale) && this->skin == other.skin &&
         Equals(this->translation, other.translation) &&
         Equals(this->weights, other.weights);
}
bool OrthographicCamera::operator==(const OrthographicCamera &other) const {
  return this->extensions == other.extensions && this->extras == other.extras &&
         TINYGLTF_DOUBLE_EQUAL(this->xmag, other.xmag) &&
         TINYGLTF_DOUBLE_EQUAL(this->ymag, other.ymag) &&
         TINYGLTF_DOUBLE_EQUAL(this->zfar, other.zfar) &&
         TINYGLTF_DOUBLE_EQUAL(this->znear, other.znear);
}
bool Parameter::operator==(const Parameter &other) const {
  if (this->bool_value != other.bool_value ||
      this->has_number_value != other.has_number_value)
    return false;

  if (!TINYGLTF_DOUBLE_EQUAL(this->number_value, other.number_value))
    return false;

  if (this->json_double_value.size() != other.json_double_value.size())
    return false;
  for (auto &it : this->json_double_value) {
    auto otherIt = other.json_double_value.find(it.first);
    if (otherIt == other.json_double_value.end()) return false;

    if (!TINYGLTF_DOUBLE_EQUAL(it.second, otherIt->second)) return false;
  }

  if (!Equals(this->number_array, other.number_array)) return false;

  if (this->string_value != other.string_value) return false;

  return true;
}
bool PerspectiveCamera::operator==(const PerspectiveCamera &other) const {
  return TINYGLTF_DOUBLE_EQUAL(this->aspectRatio, other.aspectRatio) &&
         this->extensions == other.extensions && this->extras == other.extras &&
         TINYGLTF_DOUBLE_EQUAL(this->yfov, other.yfov) &&
         TINYGLTF_DOUBLE_EQUAL(this->zfar, other.zfar) &&
         TINYGLTF_DOUBLE_EQUAL(this->znear, other.znear);
}
bool Primitive::operator==(const Primitive &other) const {
  return this->attributes == other.attributes && this->extras == other.extras &&
         this->indices == other.indices && this->material == other.material &&
         this->mode == other.mode && this->targets == other.targets;
}
bool Sampler::operator==(const Sampler &other) const {
  return this->extras == other.extras && this->magFilter == other.magFilter &&
         this->minFilter == other.minFilter && this->name == other.name &&
         this->wrapR == other.wrapR && this->wrapS == other.wrapS &&
         this->wrapT == other.wrapT;
}
bool Scene::operator==(const Scene &other) const {
  return this->extensions == other.extensions && this->extras == other.extras &&
         this->name == other.name && this->nodes == other.nodes;
  ;
}
bool Skin::operator==(const Skin &other) const {
  return this->inverseBindMatrices == other.inverseBindMatrices &&
         this->joints == other.joints && this->name == other.name &&
         this->skeleton == other.skeleton;
}
bool Texture::operator==(const Texture &other) const {
  return this->extensions == other.extensions && this->extras == other.extras &&
         this->name == other.name && this->sampler == other.sampler &&
         this->source == other.source;
}
bool Value::operator==(const Value &other) const {
  return Equals(*this, other);
}

static void swap4(unsigned int *val) {
#ifdef TINYGLTF_LITTLE_ENDIAN
  (void)val;
#else
  unsigned int tmp = *val;
  unsigned char *dst = reinterpret_cast<unsigned char *>(val);
  unsigned char *src = reinterpret_cast<unsigned char *>(&tmp);

  dst[0] = src[3];
  dst[1] = src[2];
  dst[2] = src[1];
  dst[3] = src[0];
#endif
}

static ptl::string JoinPath(const ptl::string &path0,
                            const ptl::string &path1) {
  if (path0.empty()) {
    return path1;
  } else {
    
    char lastChar = *path0.rbegin();
    if (lastChar != '/') {
      return path0 + ptl::string("/") + path1;
    } else {
      return path0 + path1;
    }
  }
}

static ptl::string FindFile(const ptl::vector<ptl::string> &paths,
                            const ptl::string &filepath, FsCallbacks *fs) {
  if (fs == nullptr || fs->ExpandFilePath == nullptr ||
      fs->FileExists == nullptr) {
    
    return ptl::string();
  }

  for (size_t i = 0; i < paths.size(); i++) {
    ptl::string absPath =
        fs->ExpandFilePath(JoinPath(paths[i], filepath), fs->user_data);
    if (fs->FileExists(absPath, fs->user_data)) {
      return absPath;
    }
  }

  return ptl::string();
}

static ptl::string GetFilePathExtension(const ptl::string &FileName) {
  if (FileName.find_last_of(".") != ptl::string::npos)
    return FileName.substr(FileName.find_last_of(".") + 1);
  return "";
}

static ptl::string GetBaseDir(const ptl::string &filepath) {
  if (filepath.find_last_of("/\\") != ptl::string::npos)
    return filepath.substr(0, filepath.find_last_of("/\\"));
  return "";
}


static ptl::string GetBaseFilename(const ptl::string &filepath) {
  return filepath.substr(filepath.find_last_of("/\\") + 1);
}

ptl::string base64_encode(unsigned char const *, unsigned int len);
ptl::string base64_decode(ptl::string const &s);



#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wexit-time-destructors"
#pragma clang diagnostic ignored "-Wglobal-constructors"
#pragma clang diagnostic ignored "-Wsign-conversion"
#pragma clang diagnostic ignored "-Wconversion"
#endif
static const ptl::string base64_chars =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";

static inline bool is_base64(unsigned char c) {
  return (isalnum(c) || (c == '+') || (c == '/'));
}

ptl::string base64_encode(unsigned char const *bytes_to_encode,
                          unsigned int in_len) {
  ptl::string ret;
  int i = 0;
  int j = 0;
  unsigned char char_array_3[3];
  unsigned char char_array_4[4];

  while (in_len--) {
    char_array_3[i++] = *(bytes_to_encode++);
    if (i == 3) {
      char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
      char_array_4[1] =
          ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
      char_array_4[2] =
          ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
      char_array_4[3] = char_array_3[2] & 0x3f;

      for (i = 0; (i < 4); i++) ret += base64_chars[char_array_4[i]];
      i = 0;
    }
  }

  if (i) {
    for (j = i; j < 3; j++) char_array_3[j] = '\0';

    char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
    char_array_4[1] =
        ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
    char_array_4[2] =
        ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);

    for (j = 0; (j < i + 1); j++) ret += base64_chars[char_array_4[j]];

    while ((i++ < 3)) ret += '=';
  }

  return ret;
}

ptl::string base64_decode(ptl::string const &encoded_string) {
  int in_len = static_cast<int>(encoded_string.size());
  int i = 0;
  int j = 0;
  int in_ = 0;
  unsigned char char_array_4[4], char_array_3[3];
  ptl::string ret;

  while (in_len-- && (encoded_string[in_] != '=') &&
         is_base64(encoded_string[in_])) {
    char_array_4[i++] = encoded_string[in_];
    in_++;
    if (i == 4) {
      for (i = 0; i < 4; i++)
        char_array_4[i] =
            static_cast<unsigned char>(base64_chars.find(char_array_4[i]));

      char_array_3[0] =
          (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
      char_array_3[1] =
          ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
      char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

      for (i = 0; (i < 3); i++) ret += char_array_3[i];
      i = 0;
    }
  }

  if (i) {
    for (j = i; j < 4; j++) char_array_4[j] = 0;

    for (j = 0; j < 4; j++)
      char_array_4[j] =
          static_cast<unsigned char>(base64_chars.find(char_array_4[j]));

    char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
    char_array_3[1] =
        ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
    char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

    for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
  }

  return ret;
}
#ifdef __clang__
#pragma clang diagnostic pop
#endif

static bool LoadExternalFile(ptl::vector<unsigned char> *out, ptl::string *err,
                             ptl::string *warn, const ptl::string &filename,
                             const ptl::string &basedir, bool required,
                             size_t reqBytes, bool checkSize, FsCallbacks *fs) {
  if (fs == nullptr || fs->FileExists == nullptr ||
      fs->ExpandFilePath == nullptr || fs->ReadWholeFile == nullptr) {
    
    if (err) {
      (*err) += "FS callback[s] not set\n";
    }
    return false;
  }

  ptl::string *failMsgOut = required ? err : warn;

  out->clear();

  ptl::vector<ptl::string> paths;
  paths.push_back(basedir);
  paths.push_back(".");

  ptl::string filepath = FindFile(paths, filename, fs);
  if (filepath.empty() || filename.empty()) {
    if (failMsgOut) {
      (*failMsgOut) += "File not found : " + filename + "\n";
    }
    return false;
  }

  ptl::vector<unsigned char> buf;
  ptl::string fileReadErr;
  bool fileRead =
      fs->ReadWholeFile(&buf, &fileReadErr, filepath, fs->user_data);
  if (!fileRead) {
    if (failMsgOut) {
      (*failMsgOut) +=
          "File read error : " + filepath + " : " + fileReadErr + "\n";
    }
    return false;
  }

  size_t sz = buf.size();
  if (sz == 0) {
    if (failMsgOut) {
      (*failMsgOut) += "File is empty : " + filepath + "\n";
    }
    return false;
  }

  if (checkSize) {
    if (reqBytes == sz) {
      out->swap(buf);
      return true;
    } else {
      ptl::stringstream ss;
      ss << "File size mismatch : " << filepath << ", requestedBytes "
         << reqBytes << ", but got " << sz << std::endl;
      if (failMsgOut) {
        (*failMsgOut) += ss.str();
      }
      return false;
    }
  }

  out->swap(buf);
  return true;
}

void TinyGLTF::SetImageLoader(LoadImageDataFunction func, void *user_data) {
  LoadImageData = func;
  load_image_user_data_ = user_data;
}

#ifndef TINYGLTF_NO_STB_IMAGE
bool LoadImageData(Image *image, const int image_idx, ptl::string *err,
                   ptl::string *warn, int req_width, int req_height,
                   const unsigned char *bytes, int size, void *user_data) {
  (void)user_data;
  (void)warn;

  int w = 0, h = 0, comp = 0, req_comp = 0;

  unsigned char *data = nullptr;
  
  
  req_comp = 4;
  int bits = 8;
  int pixel_type = TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE;

  
  
  
  
  
  
  if (stbi_is_16_bit_from_memory(bytes, size)) {
    data = (unsigned char *)stbi_load_16_from_memory(bytes, size, &w, &h, &comp,
                                                     req_comp);
    if (data) {
      bits = 16;
      pixel_type = TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT;
    }
  }

  
  
  
  
  
  
  
  
  

  if (!data) data = stbi_load_from_memory(bytes, size, &w, &h, &comp, req_comp);
  if (!data) {
    
    if (err) {
      (*err) +=
          "Unknown image format. STB cannot decode image data for image[" +
          ptl::to_string(image_idx) + "] name = \"" + image->name + "\".\n";
    }
    
    FILE* fp;
    errno_t err = fopen_s(&fp,"./hostapp/data/Textures/UV_Test_Pattern.png", "r");
    data = stbi_load_from_file(fp, &w, &h, &comp, req_comp);
    if(!data)
        return false;
  }

  if (w < 1 || h < 1) {
    stbi_image_free(data);
    if (err) {
      (*err) += "Invalid image data for image[" + ptl::to_string(image_idx) +
                "] name = \"" + image->name + "\"\n";
    }
    return false;
  }

  if (req_width > 0) {
    if (req_width != w) {
      stbi_image_free(data);
      if (err) {
        (*err) += "Image width mismatch for image[" +
                  ptl::to_string(image_idx) + "] name = \"" + image->name +
                  "\"\n";
      }
      return false;
    }
  }

  if (req_height > 0) {
    if (req_height != h) {
      stbi_image_free(data);
      if (err) {
        (*err) += "Image height mismatch. for image[" +
                  ptl::to_string(image_idx) + "] name = \"" + image->name +
                  "\"\n";
      }
      return false;
    }
  }

  image->width = w;
  image->height = h;
  image->component = req_comp;
  image->bits = bits;
  image->pixel_type = pixel_type;
  image->image.resize(static_cast<size_t>(w * h * req_comp) * (bits / 8));
  std::copy(data, data + w * h * req_comp * (bits / 8), image->image.begin());
  stbi_image_free(data);

  return true;
}
#endif

void TinyGLTF::SetImageWriter(WriteImageDataFunction func, void *user_data) {
  WriteImageData = func;
  write_image_user_data_ = user_data;
}

#ifndef TINYGLTF_NO_STB_IMAGE_WRITE
static void WriteToMemory_stbi(void *context, void *data, int size) {
  ptl::vector<unsigned char> *buffer =
      reinterpret_cast<ptl::vector<unsigned char> *>(context);

  unsigned char *pData = reinterpret_cast<unsigned char *>(data);

  buffer->insert(buffer->end(), pData, pData + size);
}

bool WriteImageData(const ptl::string *basepath, const ptl::string *filename,
                    Image *image, bool embedImages, void *fsPtr) {
  const ptl::string ext = GetFilePathExtension(*filename);

  
  ptl::string header;
  ptl::vector<unsigned char> data;

  if (ext == "png") {
    if ((image->bits != 8) ||
        (image->pixel_type != TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE)) {
      
      return false;
    }

    if (!stbi_write_png_to_func(WriteToMemory_stbi, &data, image->width,
                                image->height, image->component,
                                &image->image[0], 0)) {
      return false;
    }
    header = "data:image/png;base64,";
  } else if (ext == "jpg") {
    if (!stbi_write_jpg_to_func(WriteToMemory_stbi, &data, image->width,
                                image->height, image->component,
                                &image->image[0], 100)) {
      return false;
    }
    header = "data:image/jpeg;base64,";
  } else if (ext == "bmp") {
    if (!stbi_write_bmp_to_func(WriteToMemory_stbi, &data, image->width,
                                image->height, image->component,
                                &image->image[0])) {
      return false;
    }
    header = "data:image/bmp;base64,";
  } else if (!embedImages) {
    
    return false;
  }

  if (embedImages) {
    
    if (data.size()) {
      image->uri =
          header +
          base64_encode(&data[0], static_cast<unsigned int>(data.size()));
    } else {
      
    }
  } else {
    
    FsCallbacks *fs = reinterpret_cast<FsCallbacks *>(fsPtr);
    if ((fs != nullptr) && (fs->WriteWholeFile != nullptr)) {
      const ptl::string imagefilepath = JoinPath(*basepath, *filename);
      ptl::string writeError;
      if (!fs->WriteWholeFile(&writeError, imagefilepath, data,
                              fs->user_data)) {
        
        return false;
      }
    } else {
      
    }
    image->uri = *filename;
  }

  return true;
}
#endif

void TinyGLTF::SetFsCallbacks(FsCallbacks callbacks) { fs = callbacks; }

#ifndef TINYGLTF_NO_FS


bool FileExists(const ptl::string &abs_filename, void *) {
  bool ret;
#ifdef TINYGLTF_ANDROID_LOAD_FROM_ASSETS
  if (asset_manager) {
    AAsset *asset = AAssetManager_open(asset_manager, abs_filename.c_str(),
                                       AASSET_MODE_STREAMING);
    if (!asset) {
      return false;
    }
    AAsset_close(asset);
    ret = true;
  } else {
    return false;
  }
#else
#ifdef _WIN32
  FILE *fp;
  errno_t err = fopen_s(&fp, abs_filename.c_str(), "rb");
  if (err != 0) {
    return false;
  }
#else
  FILE *fp = fopen(abs_filename.c_str(), "rb");
#endif
  if (fp) {
    ret = true;
    fclose(fp);
  } else {
    ret = false;
  }
#endif

  return ret;
}

ptl::string ExpandFilePath(const ptl::string &filepath, void *) {

#ifdef _WIN32
  DWORD len = ExpandEnvironmentStringsA(filepath.c_str(), NULL, 0);
  char *str = new char[len];
  ExpandEnvironmentStringsA(filepath.c_str(), str, len);

  ptl::string s(str);

  delete[] str;

  return s;
#else

#if defined(TARGET_OS_IPHONE) || defined(TARGET_IPHONE_SIMULATOR) || \
    defined(__ANDROID__) || defined(__EMSCRIPTEN__)
  
  ptl::string s = filepath;
#elif defined(PLATFORM_PS)
	ptl::string s = filepath;
#else
  ptl::string s;
  wordexp_t p;

  if (filepath.empty()) {
    return "";
  }

  
  int ret = wordexp(filepath.c_str(), &p, 0);
  if (ret) {
    
    s = filepath;
    return s;
  }

  
  if (p.we_wordv) {
    s = ptl::string(p.we_wordv[0]);
    wordfree(&p);
  } else {
    s = filepath;
  }

#endif

  return s;
#endif

}

bool ReadWholeFile(ptl::vector<unsigned char> *out, ptl::string *err,
                   const ptl::string &filepath, void *) {
#ifdef TINYGLTF_ANDROID_LOAD_FROM_ASSETS
  if (asset_manager) {
    AAsset *asset = AAssetManager_open(asset_manager, filepath.c_str(),
                                       AASSET_MODE_STREAMING);
    if (!asset) {
      if (err) {
        (*err) += "File open error : " + filepath + "\n";
      }
      return false;
    }
    size_t size = AAsset_getLength(asset);
    if (size <= 0) {
      if (err) {
        (*err) += "Invalid file size : " + filepath +
                  " (does the path point to a directory?)";
      }
    }
    out->resize(size);
    AAsset_read(asset, reinterpret_cast<char *>(&out->at(0)), size);
    AAsset_close(asset);
    return true;
  } else {
    if (err) {
      (*err) += "No asset manager specified : " + filepath + "\n";
    }
    return false;
  }
#else
  std::ifstream f(filepath.c_str(), std::ifstream::binary);
  if (!f) {
    if (err) {
      (*err) += "File open error : " + filepath + "\n";
    }
    return false;
  }

  f.seekg(0, f.end);
  size_t sz = static_cast<size_t>(f.tellg());
  f.seekg(0, f.beg);

  if (int(sz) < 0) {
    if (err) {
      (*err) += "Invalid file size : " + filepath +
                " (does the path point to a directory?)";
    }
    return false;
  } else if (sz == 0) {
    if (err) {
      (*err) += "File is empty : " + filepath + "\n";
    }
    return false;
  }

  out->resize(sz);
  f.read(reinterpret_cast<char *>(&out->at(0)),
         static_cast<std::streamsize>(sz));
  f.close();

  return true;
#endif
}

bool WriteWholeFile(ptl::string *err, const ptl::string &filepath,
                    const ptl::vector<unsigned char> &contents, void *) {
  std::ofstream f(filepath.c_str(), std::ofstream::binary);
  if (!f) {
    if (err) {
      (*err) += "File open error for writing : " + filepath + "\n";
    }
    return false;
  }

  f.write(reinterpret_cast<const char *>(&contents.at(0)),
          static_cast<std::streamsize>(contents.size()));
  if (!f) {
    if (err) {
      (*err) += "File write error: " + filepath + "\n";
    }
    return false;
  }

  f.close();
  return true;
}

#endif  

static ptl::string MimeToExt(const ptl::string &mimeType) {
  if (mimeType == "image/jpeg") {
    return "jpg";
  } else if (mimeType == "image/png") {
    return "png";
  } else if (mimeType == "image/bmp") {
    return "bmp";
  } else if (mimeType == "image/gif") {
    return "gif";
  }

  return "";
}

static void UpdateImageObject(Image &image, ptl::string &baseDir, int index,
                              bool embedImages,
                              WriteImageDataFunction *WriteImageData = nullptr,
                              void *user_data = nullptr) {
  ptl::string filename;
  ptl::string ext;

  
  if (image.uri.size()) {
    filename = GetBaseFilename(image.uri);
    ext = GetFilePathExtension(filename);

  } else if (image.name.size()) {
    ext = MimeToExt(image.mimeType);
    
    filename = image.name + "." + ext;
  } else {
    ext = MimeToExt(image.mimeType);
    
    filename = ptl::to_string(index) + "." + ext;
  }

  
  if (*WriteImageData != nullptr) {
    ptl::string uri;
    (*WriteImageData)(&baseDir, &filename, &image, embedImages, user_data);
  }
}

bool IsDataURI(const ptl::string &in) {
  ptl::string header = "data:application/octet-stream;base64,";
  if (in.find(header) == 0) {
    return true;
  }

  header = "data:image/jpeg;base64,";
  if (in.find(header) == 0) {
    return true;
  }

  header = "data:image/png;base64,";
  if (in.find(header) == 0) {
    return true;
  }

  header = "data:image/bmp;base64,";
  if (in.find(header) == 0) {
    return true;
  }

  header = "data:image/gif;base64,";
  if (in.find(header) == 0) {
    return true;
  }

  header = "data:text/plain;base64,";
  if (in.find(header) == 0) {
    return true;
  }

  header = "data:application/gltf-buffer;base64,";
  if (in.find(header) == 0) {
    return true;
  }

  return false;
}

bool DecodeDataURI(ptl::vector<unsigned char> *out, ptl::string &mime_type,
                   const ptl::string &in, size_t reqBytes, bool checkSize) {
  ptl::string header = "data:application/octet-stream;base64,";
  ptl::string data;
  if (in.find(header) == 0) {
    data = base64_decode(in.substr(header.size()));  
  }

  if (data.empty()) {
    header = "data:image/jpeg;base64,";
    if (in.find(header) == 0) {
      mime_type = "image/jpeg";
      data = base64_decode(in.substr(header.size()));  
    }
  }

  if (data.empty()) {
    header = "data:image/png;base64,";
    if (in.find(header) == 0) {
      mime_type = "image/png";
      data = base64_decode(in.substr(header.size()));  
    }
  }

  if (data.empty()) {
    header = "data:image/bmp;base64,";
    if (in.find(header) == 0) {
      mime_type = "image/bmp";
      data = base64_decode(in.substr(header.size()));  
    }
  }

  if (data.empty()) {
    header = "data:image/gif;base64,";
    if (in.find(header) == 0) {
      mime_type = "image/gif";
      data = base64_decode(in.substr(header.size()));  
    }
  }

  if (data.empty()) {
    header = "data:text/plain;base64,";
    if (in.find(header) == 0) {
      mime_type = "text/plain";
      data = base64_decode(in.substr(header.size()));
    }
  }

  if (data.empty()) {
    header = "data:application/gltf-buffer;base64,";
    if (in.find(header) == 0) {
      data = base64_decode(in.substr(header.size()));
    }
  }

  if (data.empty()) {
    return false;
  }

  if (checkSize) {
    if (data.size() != reqBytes) {
      return false;
    }
    out->resize(reqBytes);
  } else {
    out->resize(data.size());
  }
  std::copy(data.begin(), data.end(), out->begin());
  return true;
}

static bool ParseJsonAsValue(Value *ret, const json::Value &o) {
  Value val{};
  switch (o.GetType()) {
    case rapidjson::Type::kObjectType: {
      Value::Object value_object;
      for (auto it = o.MemberBegin(); it != o.MemberEnd(); it++) {
        Value entry;
        ParseJsonAsValue(&entry, it->value);
        if (entry.Type() != NULL_TYPE)
          value_object[it->name.GetString()] = entry;
      }
      if (value_object.size() > 0) val = Value(value_object);
    } break;
    case rapidjson::Type::kArrayType: {
      Value::Array value_array;
      for (auto it = o.Begin(); it != o.End(); it++) {
        Value entry;
        ParseJsonAsValue(&entry, *it);
        if (entry.Type() != NULL_TYPE) value_array.push_back(entry);
      }
      if (value_array.size() > 0) val = Value(value_array);
    } break;
    case rapidjson::Type::kStringType:
      val = Value(o.GetString());
      break;
    case rapidjson::Type::kTrueType:
    case rapidjson::Type::kFalseType:
      val = Value(o.Get<bool>());
      break;
    case rapidjson::Type::kNumberType:
      if (o.IsInt() || o.IsUint()) {
        val = Value(static_cast<int>(o.Get<int64_t>()));
      } else {
        val = Value(o.Get<double>());
      }
      break;
    case rapidjson::Type::kNullType:
      
      break;
  }
  if (ret) *ret = val;

  return val.Type() != NULL_TYPE;
}

static bool ParseExtrasProperty(Value *ret, const json::Value &o) {
  json::const_iterator it = o.FindMember("extras");
  if (it == o.MemberEnd()) {
    return false;
  }

  return ParseJsonAsValue(ret, it->value);
}

static bool ParseBooleanProperty(bool *ret, ptl::string *err,
                                 const json::Value &o,
                                 const ptl::string &property,
                                 const bool required,
                                 const ptl::string &parent_node = "") {
  json::const_iterator it = o.FindMember(property.c_str());
  if (it == o.MemberEnd()) {
    if (required) {
      if (err) {
        (*err) += "'" + property + "' property is missing";
        if (!parent_node.empty()) {
          (*err) += " in " + parent_node;
        }
        (*err) += ".\n";
      }
    }
    return false;
  }

  if (!it->value.IsBool()) {
    if (required) {
      if (err) {
        (*err) += "'" + property + "' property is not a bool type.\n";
      }
    }
    return false;
  }

  if (ret) {
    (*ret) = it->value.GetBool();
  }

  return true;
}

static bool ParseNumberProperty(double *ret, ptl::string *err,
                                const json::Value &o,
                                const ptl::string &property,
                                const bool required,
                                const ptl::string &parent_node = "") {
  json::const_iterator it = o.FindMember(property.c_str());
  if (it == o.MemberEnd()) {
    if (required) {
      if (err) {
        (*err) += "'" + property + "' property is missing";
        if (!parent_node.empty()) {
          (*err) += " in " + parent_node;
        }
        (*err) += ".\n";
      }
    }
    return false;
  }

  if (!it->value.IsNumber()) {
    if (required) {
      if (err) {
        (*err) += "'" + property + "' property is not a number type.\n";
      }
    }
    return false;
  }

  if (ret) {
    (*ret) = it->value.GetDouble();
  }

  return true;
}

static bool ParseNumberArrayProperty(ptl::vector<double> *ret, ptl::string *err,
                                     const json::Value &o,
                                     const ptl::string &property, bool required,
                                     const ptl::string &parent_node = "") {
  json::const_iterator it = o.FindMember(property.c_str());
  if (it == o.MemberEnd()) {
    if (required) {
      if (err) {
        (*err) += "'" + property + "' property is missing";
        if (!parent_node.empty()) {
          (*err) += " in " + parent_node;
        }
        (*err) += ".\n";
      }
    }
    return false;
  }

  if (!it->value.IsArray()) {
    if (required) {
      if (err) {
        (*err) += "'" + property + "' property is not an array";
        if (!parent_node.empty()) {
          (*err) += " in " + parent_node;
        }
        (*err) += ".\n";
      }
    }
    return false;
  }

  ret->clear();
  for (auto i = it->value.Begin(); i != it->value.End(); i++) {
    if (!i->IsNumber()) {
      if (required) {
        if (err) {
          (*err) += "'" + property + "' property is not a number.\n";
          if (!parent_node.empty()) {
            (*err) += " in " + parent_node;
          }
          (*err) += ".\n";
        }
      }
      return false;
    }
    ret->push_back(i->GetDouble());
  }

  return true;
}

static bool ParseStringProperty(
    ptl::string *ret, ptl::string *err, const json::Value &o,
    const ptl::string &property, bool required,
    const ptl::string &parent_node = ptl::string()) {
  json::const_iterator it = o.FindMember(property.c_str());
  if (it == o.MemberEnd()) {
    if (required) {
      if (err) {
        (*err) += "'" + property + "' property is missing";
        if (parent_node.empty()) {
          (*err) += ".\n";
        } else {
          (*err) += " in `" + parent_node + "'.\n";
        }
      }
    }
    return false;
  }

  if (!it->value.IsString()) {
    if (required) {
      if (err) {
        (*err) += "'" + property + "' property is not a string type.\n";
      }
    }
    return false;
  }

  if (ret) {
    (*ret) = it->value.GetString();
  }

  return true;
}

static bool ParseStringIntProperty(ptl::map<ptl::string, int> *ret,
                                   ptl::string *err, const json::Value &o,
                                   const ptl::string &property, bool required,
                                   const ptl::string &parent = "") {
  json::const_iterator it = o.FindMember(property.c_str());
  if (it == o.MemberEnd()) {
    if (required) {
      if (err) {
        if (!parent.empty()) {
          (*err) +=
              "'" + property + "' property is missing in " + parent + ".\n";
        } else {
          (*err) += "'" + property + "' property is missing.\n";
        }
      }
    }
    return false;
  }

  
  if (!it->value.IsObject()) {
    if (required) {
      if (err) {
        (*err) += "'" + property + "' property is not an object.\n";
      }
    }
    return false;
  }

  ret->clear();
  const json::Value &dict = it->value;

  json::const_iterator dictIt(dict.MemberBegin());
  json::const_iterator dictItEnd(dict.MemberEnd());

  for (; dictIt != dictItEnd; ++dictIt) {
    if (!dictIt->value.IsNumber()) {
      if (required) {
        if (err) {
          (*err) += "'" + property + "' value is not an int.\n";
        }
      }
      return false;
    }

    
    (*ret)[dictIt->name.GetString()] = dictIt->value.GetInt();
  }
  return true;
}

static bool ParseJSONProperty(ptl::map<ptl::string, double> *ret,
                              ptl::string *err, const json::Value &o,
                              const ptl::string &property, bool required) {
  json::const_iterator it = o.FindMember(property.c_str());
  if (it == o.MemberEnd()) {
    if (required) {
      if (err) {
        (*err) += "'" + property + "' property is missing. \n'";
      }
    }
    return false;
  }

  if (!it->value.IsObject()) {
    if (required) {
      if (err) {
        (*err) += "'" + property + "' property is not a JSON object.\n";
      }
    }
    return false;
  }

  ret->clear();
  const json::Value &obj = it->value;
  json::const_iterator it2(obj.MemberBegin());
  json::const_iterator itEnd(obj.MemberEnd());
  for (; it2 != itEnd; it2++) {
    if (it2->value.IsNumber())
      ret->insert(std::pair<ptl::string, double>(it2->name.GetString(),
                                                 it2->value.GetDouble()));
  }

  return true;
}

static bool ParseParameterProperty(Parameter *param, ptl::string *err,
                                   const json::Value &o,
                                   const ptl::string &prop, bool required) {
  
  
  
  
  
  if (ParseStringProperty(&param->string_value, err, o, prop, false)) {
    
    return true;
  } else if (ParseNumberArrayProperty(&param->number_array, err, o, prop,
                                      false)) {
    
    return true;
  } else if (ParseNumberProperty(&param->number_value, err, o, prop, false)) {
    return param->has_number_value = true;
  } else if (ParseJSONProperty(&param->json_double_value, err, o, prop,
                               false)) {
    return true;
  } else if (ParseBooleanProperty(&param->bool_value, err, o, prop, false)) {
    return true;
  } else {
    if (required) {
      if (err) {
        (*err) += "parameter must be a string or number / number array.\n";
      }
    }
    return false;
  }
}

static bool ParseExtensionsProperty(ExtensionMap *ret, ptl::string *err,
                                    const json::Value &o) {
  (void)err;

  json::const_iterator it = o.FindMember("extensions");
  if (it == o.MemberEnd()) {
    return false;
  }
  if (!it->value.IsObject()) {
    return false;
  }
  ExtensionMap extensions;
  json::const_iterator extIt = it->value.MemberBegin();
  for (; extIt != it->value.MemberEnd(); extIt++) {
    if (!extIt->value.IsObject()) continue;
    if (!ParseJsonAsValue(&extensions[extIt->name.GetString()], extIt->value)) {
      if (!extIt->value.ObjectEmpty()) {
        
        
        extensions[extIt->name.GetString()] = Value{Value::Object{}};
      }
    }
  }
  if (ret) {
    (*ret) = extensions;
  }
  return true;
}

static bool ParseAsset(Asset *asset, ptl::string *err, const json::Value &o) {
  ParseStringProperty(&asset->version, err, o, "version", true, "Asset");
  ParseStringProperty(&asset->generator, err, o, "generator", false, "Asset");
  ParseStringProperty(&asset->minVersion, err, o, "minVersion", false, "Asset");

  ParseExtensionsProperty(&asset->extensions, err, o);

  
  ParseExtrasProperty(&(asset->extras), o);

  return true;
}

static bool ParseImage(Image *image, const int image_idx, ptl::string *err,
                       ptl::string *warn, const json::Value &o,
                       const ptl::string &basedir, FsCallbacks *fs,
                       LoadImageDataFunction *LoadImageData = nullptr,
                       void *load_image_user_data = nullptr) {
  

  
  
  bool hasBufferView = (o.FindMember("bufferView") != o.MemberEnd());
  bool hasURI = (o.FindMember("uri") != o.MemberEnd());

  ParseStringProperty(&image->name, err, o, "name", false);
  if (hasBufferView && hasURI) {
    
    if (err) {
      (*err) +=
          "Only one of `bufferView` or `uri` should be defined, but both are "
          "defined for image[" +
          ptl::to_string(image_idx) + "] name = \"" + image->name + "\"\n";
    }
    return false;
  }

  if (!hasBufferView && !hasURI) {
    if (err) {
      (*err) += "Neither required `bufferView` nor `uri` defined for image[" +
                ptl::to_string(image_idx) + "] name = \"" + image->name +
                "\"\n";
    }
    return false;
  }

  ParseExtensionsProperty(&image->extensions, err, o);
  ParseExtrasProperty(&image->extras, o);
  if (hasBufferView) {
    double bufferView = -1;
    if (!ParseNumberProperty(&bufferView, err, o, "bufferView", true)) {
      if (err) {
        (*err) += "Failed to parse `bufferView` for image[" +
                  ptl::to_string(image_idx) + "] name = \"" + image->name +
                  "\"\n";
      }
      return false;
    }

    ptl::string mime_type;
    ParseStringProperty(&mime_type, err, o, "mimeType", false);

    double width = 0.0;
    ParseNumberProperty(&width, err, o, "width", false);

    double height = 0.0;
    ParseNumberProperty(&height, err, o, "height", false);

    
    
    image->bufferView = static_cast<int>(bufferView);
    image->mimeType = mime_type;
    image->width = static_cast<int>(width);
    image->height = static_cast<int>(height);
    return true;
  }

  


  ptl::string uri;
  ptl::string tmp_err;
  if (!ParseStringProperty(&uri, &tmp_err, o, "uri", true)) {
    if (err) {
      (*err) += "Failed to parse `uri` for image[" + ptl::to_string(image_idx) +
                "] name = \"" + image->name + "\".\n";
    }
    return false;
  }

  ptl::vector<unsigned char> img;

  if (IsDataURI(uri)) {
    if (!DecodeDataURI(&img, image->mimeType, uri, 0, false)) {
      if (err) {
        (*err) += "Failed to decode 'uri' for image[" +
                  ptl::to_string(image_idx) + "] name = [" + image->name +
                  "]\n";
      }
      return false;
    }
  } else {
    
    
    image->uri = uri;
#ifdef TINYGLTF_NO_EXTERNAL_IMAGE
    return true;
#endif
    if (!LoadExternalFile(&img, err, warn, uri, basedir, false, 0, false, fs)) {
      if (warn) {
        (*warn) += "Failed to load external 'uri' for image[" +
                   ptl::to_string(image_idx) + "] name = [" + image->name +
                   "]\n";
      }
      
      return true;
    }

    if (img.empty()) {
      if (warn) {
        (*warn) += "Image data is empty for image[" +
                   ptl::to_string(image_idx) + "] name = [" + image->name +
                   "] \n";
      }
      return false;
    }
  }

  if (*LoadImageData == nullptr) {
    if (err) {
      (*err) += "No LoadImageData callback specified.\n";
    }
    return false;
  }
  return (*LoadImageData)(image, image_idx, err, warn, 0, 0, &img.at(0),
                          static_cast<int>(img.size()), load_image_user_data);
}

static bool ParseTexture(Texture *texture, ptl::string *err,
                         const json::Value &o, const ptl::string &basedir) {
  (void)basedir;
  double sampler = -1.0;
  double source = -1.0;
  ParseNumberProperty(&sampler, err, o, "sampler", false);

  ParseNumberProperty(&source, err, o, "source", false);

  texture->sampler = static_cast<int>(sampler);
  texture->source = static_cast<int>(source);

  ParseExtensionsProperty(&texture->extensions, err, o);
  ParseExtrasProperty(&texture->extras, o);

  ParseStringProperty(&texture->name, err, o, "name", false);

  return true;
}

static bool ParseBuffer(Buffer *buffer, ptl::string *err, const json::Value &o,
                        FsCallbacks *fs, const ptl::string &basedir,
                        bool is_binary = false,
                        const unsigned char *bin_data = nullptr,
                        size_t bin_size = 0) {
  double byteLength;
  if (!ParseNumberProperty(&byteLength, err, o, "byteLength", true, "Buffer")) {
    return false;
  }

  
  buffer->uri.clear();
  ParseStringProperty(&buffer->uri, err, o, "uri", false, "Buffer");

  
  if (!is_binary && buffer->uri.empty()) {
    if (err) {
      (*err) += "'uri' is missing from non binary glTF file buffer.\n";
    }
  }

  json::const_iterator type = o.FindMember("type");
  if (type != o.MemberEnd()) {
    if (type->value.IsString()) {
      const ptl::string &ty = type->value.GetString();
      if (ty.compare("arraybuffer") == 0) {
        
      }
    }
  }

  size_t bytes = static_cast<size_t>(byteLength);
  if (is_binary) {
    
    if (!buffer->uri.empty()) {
      
      if (IsDataURI(buffer->uri)) {
        ptl::string mime_type;
        if (!DecodeDataURI(&buffer->data, mime_type, buffer->uri, bytes,
                           true)) {
          if (err) {
            (*err) +=
                "Failed to decode 'uri' : " + buffer->uri + " in Buffer\n";
          }
          return false;
        }
      } else {
        
        if (!LoadExternalFile(&buffer->data, err,  nullptr,
                              buffer->uri, basedir, true, bytes, true, fs)) {
          return false;
        }
      }
    } else {
      

      if ((bin_size == 0) || (bin_data == nullptr)) {
        if (err) {
          (*err) += "Invalid binary data in `Buffer'.\n";
        }
        return false;
      }

      if (byteLength > bin_size) {
        if (err) {
          ptl::stringstream ss;
          ss << "Invalid `byteLength'. Must be equal or less than binary size: "
                "`byteLength' = "
             << byteLength << ", binary size = " << bin_size << std::endl;
          (*err) += ss.str();
        }
        return false;
      }

      
      buffer->data.resize(static_cast<size_t>(byteLength));
      memcpy(&(buffer->data.at(0)), bin_data, static_cast<size_t>(byteLength));
    }

  } else {
    if (IsDataURI(buffer->uri)) {
      ptl::string mime_type;
      if (!DecodeDataURI(&buffer->data, mime_type, buffer->uri, bytes, true)) {
        if (err) {
          (*err) += "Failed to decode 'uri' : " + buffer->uri + " in Buffer\n";
        }
        return false;
      }
    } else {
      
      if (!LoadExternalFile(&buffer->data, err,  nullptr, buffer->uri,
                            basedir, true, bytes, true, fs)) {
        return false;
      }
    }
  }

  ParseStringProperty(&buffer->name, err, o, "name", false);

  return true;
}

static bool ParseBufferView(BufferView *bufferView, ptl::string *err,
                            const json::Value &o) {
  double buffer = -1.0;
  if (!ParseNumberProperty(&buffer, err, o, "buffer", true, "BufferView")) {
    return false;
  }

  double byteOffset = 0.0;
  ParseNumberProperty(&byteOffset, err, o, "byteOffset", false);

  double byteLength = 1.0;
  if (!ParseNumberProperty(&byteLength, err, o, "byteLength", true,
                           "BufferView")) {
    return false;
  }

  size_t byteStride = 0;
  double byteStrideValue = 0.0;
  if (!ParseNumberProperty(&byteStrideValue, err, o, "byteStride", false)) {
    
    
    
    
    
    byteStride = 0;
  } else {
    byteStride = static_cast<size_t>(byteStrideValue);
  }

  if ((byteStride > 252) || ((byteStride % 4) != 0)) {
    if (err) {
      ptl::stringstream ss;
      ss << "Invalid `byteStride' value. `byteStride' must be the multiple of "
            "4 : "
         << byteStride << std::endl;

      (*err) += ss.str();
    }
    return false;
  }

  double target = 0.0;
  ParseNumberProperty(&target, err, o, "target", false);
  int targetValue = static_cast<int>(target);
  if ((targetValue == TINYGLTF_TARGET_ARRAY_BUFFER) ||
      (targetValue == TINYGLTF_TARGET_ELEMENT_ARRAY_BUFFER)) {
    
  } else {
    targetValue = 0;
  }
  bufferView->target = targetValue;

  ParseStringProperty(&bufferView->name, err, o, "name", false);

  bufferView->buffer = static_cast<int>(buffer);
  bufferView->byteOffset = static_cast<size_t>(byteOffset);
  bufferView->byteLength = static_cast<size_t>(byteLength);
  bufferView->byteStride = static_cast<size_t>(byteStride);
  return true;
}

static bool ParseSparseAccessor(Accessor *accessor, ptl::string *err,
                                const json::Value &o) {
  accessor->sparse.isSparse = true;

  double count = 0.0;
  ParseNumberProperty(&count, err, o, "count", true);

  const auto indices_iterator = o.FindMember("indices");
  const auto values_iterator = o.FindMember("values");
  if (indices_iterator == o.MemberEnd()) {
    (*err) = "the sparse object of this accessor doesn't have indices";
    return false;
  }

  if (values_iterator == o.MemberEnd()) {
    (*err) = "the sparse object ob ths accessor doesn't have values";
    return false;
  }

  const json::Value &indices_obj = indices_iterator->value;
  const json::Value &values_obj = values_iterator->name;

  double indices_buffer_view = 0.0, indices_byte_offset = 0.0,
         component_type = 0.0;
  ParseNumberProperty(&indices_buffer_view, err, indices_obj, "bufferView",
                      true);
  ParseNumberProperty(&indices_byte_offset, err, indices_obj, "byteOffset",
                      true);
  ParseNumberProperty(&component_type, err, indices_obj, "componentType", true);

  double values_buffer_view = 0.0, values_byte_offset = 0.0;
  ParseNumberProperty(&values_buffer_view, err, values_obj, "bufferView", true);
  ParseNumberProperty(&values_byte_offset, err, values_obj, "byteOffset", true);

  accessor->sparse.count = static_cast<int>(count);
  accessor->sparse.indices.bufferView = static_cast<int>(indices_buffer_view);
  accessor->sparse.indices.byteOffset = static_cast<int>(indices_byte_offset);
  accessor->sparse.indices.componentType = static_cast<int>(component_type);
  accessor->sparse.values.bufferView = static_cast<int>(values_buffer_view);
  accessor->sparse.values.byteOffset = static_cast<int>(values_byte_offset);

  

  return true;
}

static bool ParseAccessor(Accessor *accessor, ptl::string *err,
                          const json::Value &o) {
  double bufferView = -1.0;
  ParseNumberProperty(&bufferView, err, o, "bufferView", false, "Accessor");

  double byteOffset = 0.0;
  ParseNumberProperty(&byteOffset, err, o, "byteOffset", false, "Accessor");

  bool normalized = false;
  ParseBooleanProperty(&normalized, err, o, "normalized", false, "Accessor");

  double componentType = 0.0;
  if (!ParseNumberProperty(&componentType, err, o, "componentType", true,
                           "Accessor")) {
    return false;
  }

  double count = 0.0;
  if (!ParseNumberProperty(&count, err, o, "count", true, "Accessor")) {
    return false;
  }

  ptl::string type;
  if (!ParseStringProperty(&type, err, o, "type", true, "Accessor")) {
    return false;
  }

  if (type.compare("SCALAR") == 0) {
    accessor->type = TINYGLTF_TYPE_SCALAR;
  } else if (type.compare("VEC2") == 0) {
    accessor->type = TINYGLTF_TYPE_VEC2;
  } else if (type.compare("VEC3") == 0) {
    accessor->type = TINYGLTF_TYPE_VEC3;
  } else if (type.compare("VEC4") == 0) {
    accessor->type = TINYGLTF_TYPE_VEC4;
  } else if (type.compare("MAT2") == 0) {
    accessor->type = TINYGLTF_TYPE_MAT2;
  } else if (type.compare("MAT3") == 0) {
    accessor->type = TINYGLTF_TYPE_MAT3;
  } else if (type.compare("MAT4") == 0) {
    accessor->type = TINYGLTF_TYPE_MAT4;
  } else {
    ptl::stringstream ss;
    ss << "Unsupported `type` for accessor object. Got \"" << type << "\"\n";
    if (err) {
      (*err) += ss.str();
    }
    return false;
  }

  ParseStringProperty(&accessor->name, err, o, "name", false);

  accessor->minValues.clear();
  accessor->maxValues.clear();
  ParseNumberArrayProperty(&accessor->minValues, err, o, "min", false,
                           "Accessor");

  ParseNumberArrayProperty(&accessor->maxValues, err, o, "max", false,
                           "Accessor");

  accessor->count = static_cast<size_t>(count);
  accessor->bufferView = static_cast<int>(bufferView);
  accessor->byteOffset = static_cast<size_t>(byteOffset);
  accessor->normalized = normalized;
  {
    int comp = static_cast<int>(componentType);
    if (comp >= TINYGLTF_COMPONENT_TYPE_BYTE &&
        comp <= TINYGLTF_COMPONENT_TYPE_DOUBLE) {
      
      accessor->componentType = comp;
    } else {
      ptl::stringstream ss;
      ss << "Invalid `componentType` in accessor. Got " << comp << "\n";
      if (err) {
        (*err) += ss.str();
      }
      return false;
    }
  }

  ParseExtrasProperty(&(accessor->extras), o);

  
  const auto iterator = o.FindMember("sparse");
  if (iterator != o.MemberEnd()) {
    
    return ParseSparseAccessor(accessor, err, iterator->value);
  }

  return true;
}

#ifdef TINYGLTF_ENABLE_DRACO

static void DecodeIndexBuffer(draco::Mesh *mesh, size_t componentSize,
                              ptl::vector<uint8_t> &outBuffer) {
  if (componentSize == 4) {
    assert(sizeof(mesh->face(draco::FaceIndex(0))[0]) == componentSize);
    memcpy(outBuffer.data(), &mesh->face(draco::FaceIndex(0))[0],
           outBuffer.size());
  } else {
    size_t faceStride = componentSize * 3;
    for (draco::FaceIndex f(0); f < mesh->num_faces(); ++f) {
      const draco::Mesh::Face &face = mesh->face(f);
      if (componentSize == 2) {
        uint16_t indices[3] = {(uint16_t)face[0].value(),
                               (uint16_t)face[1].value(),
                               (uint16_t)face[2].value()};
        memcpy(outBuffer.data() + f.value() * faceStride, &indices[0],
               faceStride);
      } else {
        uint8_t indices[3] = {(uint8_t)face[0].value(),
                              (uint8_t)face[1].value(),
                              (uint8_t)face[2].value()};
        memcpy(outBuffer.data() + f.value() * faceStride, &indices[0],
               faceStride);
      }
    }
  }
}

template <typename T>
static bool GetAttributeForAllPoints(draco::Mesh *mesh,
                                     const draco::PointAttribute *pAttribute,
                                     ptl::vector<uint8_t> &outBuffer) {
  size_t byteOffset = 0;
  T values[4] = {0, 0, 0, 0};
  for (draco::PointIndex i(0); i < mesh->num_points(); ++i) {
    const draco::AttributeValueIndex val_index = pAttribute->mapped_index(i);
    if (!pAttribute->ConvertValue<T>(val_index, pAttribute->num_components(),
                                     values))
      return false;

    memcpy(outBuffer.data() + byteOffset, &values[0],
           sizeof(T) * pAttribute->num_components());
    byteOffset += sizeof(T) * pAttribute->num_components();
  }

  return true;
}

static bool GetAttributeForAllPoints(uint32_t componentType, draco::Mesh *mesh,
                                     const draco::PointAttribute *pAttribute,
                                     ptl::vector<uint8_t> &outBuffer) {
  bool decodeResult = false;
  switch (componentType) {
    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
      decodeResult =
          GetAttributeForAllPoints<uint8_t>(mesh, pAttribute, outBuffer);
      break;
    case TINYGLTF_COMPONENT_TYPE_BYTE:
      decodeResult =
          GetAttributeForAllPoints<int8_t>(mesh, pAttribute, outBuffer);
      break;
    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
      decodeResult =
          GetAttributeForAllPoints<uint16_t>(mesh, pAttribute, outBuffer);
      break;
    case TINYGLTF_COMPONENT_TYPE_SHORT:
      decodeResult =
          GetAttributeForAllPoints<int16_t>(mesh, pAttribute, outBuffer);
      break;
    case TINYGLTF_COMPONENT_TYPE_INT:
      decodeResult =
          GetAttributeForAllPoints<int32_t>(mesh, pAttribute, outBuffer);
      break;
    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
      decodeResult =
          GetAttributeForAllPoints<uint32_t>(mesh, pAttribute, outBuffer);
      break;
    case TINYGLTF_COMPONENT_TYPE_FLOAT:
      decodeResult =
          GetAttributeForAllPoints<float>(mesh, pAttribute, outBuffer);
      break;
    case TINYGLTF_COMPONENT_TYPE_DOUBLE:
      decodeResult =
          GetAttributeForAllPoints<double>(mesh, pAttribute, outBuffer);
      break;
    default:
      return false;
  }

  return decodeResult;
}

static bool ParseDracoExtension(Primitive *primitive, Model *model,
                                ptl::string *err,
                                const Value &dracoExtensionValue) {
  auto bufferViewValue = dracoExtensionValue.Get("bufferView");
  if (!bufferViewValue.IsInt()) return false;
  auto attributesValue = dracoExtensionValue.Get("attributes");
  if (!attributesValue.IsObject()) return false;

  auto attributesObject = attributesValue.Get<Value::Object>();
  int bufferView = bufferViewValue.Get<int>();

  BufferView &view = model->bufferViews[bufferView];
  Buffer &buffer = model->buffers[view.buffer];
  
  if (view.dracoDecoded) return true;
  view.dracoDecoded = true;

  const char *bufferViewData =
      reinterpret_cast<const char *>(buffer.data.data() + view.byteOffset);
  size_t bufferViewSize = view.byteLength;

  
  draco::DecoderBuffer decoderBuffer;
  decoderBuffer.Init(bufferViewData, bufferViewSize);
  draco::Decoder decoder;
  auto decodeResult = decoder.DecodeMeshFromBuffer(&decoderBuffer);
  if (!decodeResult.ok()) {
    return false;
  }
  const std::unique_ptr<draco::Mesh> &mesh = decodeResult->value;

  
  if (primitive->indices >= 0) {
    int32_t componentSize = GetComponentSizeInBytes(
        model->accessors[primitive->indices].componentType);
    Buffer decodedIndexBuffer;
    decodedIndexBuffer.data.resize(mesh->num_faces() * 3 * componentSize);

    DecodeIndexBuffer(mesh.get(), componentSize, decodedIndexBuffer.data);

    model->buffers.emplace_back(std::move(decodedIndexBuffer));

    BufferView decodedIndexBufferView;
    decodedIndexBufferView.buffer = int(model->buffers.size() - 1);
    decodedIndexBufferView.byteLength =
        int(mesh->num_faces() * 3 * componentSize);
    decodedIndexBufferView.byteOffset = 0;
    decodedIndexBufferView.byteStride = 0;
    decodedIndexBufferView.target = TINYGLTF_TARGET_ARRAY_BUFFER;
    model->bufferViews.emplace_back(std::move(decodedIndexBufferView));

    model->accessors[primitive->indices].bufferView =
        int(model->bufferViews.size() - 1);
    model->accessors[primitive->indices].count = int(mesh->num_faces() * 3);
  }

  for (const auto &attribute : attributesObject) {
    if (!attribute.second.IsInt()) return false;
    auto primitiveAttribute = primitive->attributes.find(attribute.first);
    if (primitiveAttribute == primitive->attributes.end()) return false;

    int dracoAttributeIndex = attribute.second.Get<int>();
    const auto pAttribute = mesh->GetAttributeByUniqueId(dracoAttributeIndex);
    const auto pBuffer = pAttribute->buffer();
    const auto componentType =
        model->accessors[primitiveAttribute->second].componentType;

    
    Buffer decodedBuffer;
    size_t bufferSize = mesh->num_points() * pAttribute->num_components() *
                        GetComponentSizeInBytes(componentType);
    decodedBuffer.data.resize(bufferSize);

    if (!GetAttributeForAllPoints(componentType, mesh.get(), pAttribute,
                                  decodedBuffer.data))
      return false;

    model->buffers.emplace_back(std::move(decodedBuffer));

    BufferView decodedBufferView;
    decodedBufferView.buffer = int(model->buffers.size() - 1);
    decodedBufferView.byteLength = bufferSize;
    decodedBufferView.byteOffset = pAttribute->byte_offset();
    decodedBufferView.byteStride = pAttribute->byte_stride();
    decodedBufferView.target = primitive->indices >= 0
                                   ? TINYGLTF_TARGET_ELEMENT_ARRAY_BUFFER
                                   : TINYGLTF_TARGET_ARRAY_BUFFER;
    model->bufferViews.emplace_back(std::move(decodedBufferView));

    model->accessors[primitiveAttribute->second].bufferView =
        int(model->bufferViews.size() - 1);
    model->accessors[primitiveAttribute->second].count =
        int(mesh->num_points());
  }

  return true;
}
#endif

static bool ParsePrimitive(Primitive *primitive, Model *model, ptl::string *err,
                           const json::Value &o) {
  double material = -1.0;
  ParseNumberProperty(&material, err, o, "material", false);
  primitive->material = static_cast<int>(material);

  double mode = static_cast<double>(TINYGLTF_MODE_TRIANGLES);
  ParseNumberProperty(&mode, err, o, "mode", false);

  int primMode = static_cast<int>(mode);
  primitive->mode = primMode;  

  double indices = -1.0;
  ParseNumberProperty(&indices, err, o, "indices", false);
  primitive->indices = static_cast<int>(indices);
  if (!ParseStringIntProperty(&primitive->attributes, err, o, "attributes",
                              true, "Primitive")) {
    return false;
  }

  
  json::const_iterator targetsObject = o.FindMember("targets");
  if ((targetsObject != o.MemberEnd()) && targetsObject->value.IsArray()) {
    for (json::const_iterator i = targetsObject->value.MemberBegin();
         i != targetsObject->value.MemberEnd(); i++) {
      ptl::map<ptl::string, int> targetAttribues;

      const json::Value &dict = i->value;
      json::const_iterator dictIt(dict.MemberBegin());
      json::const_iterator dictItEnd(dict.MemberEnd());

      for (; dictIt != dictItEnd; ++dictIt) {
        targetAttribues[dictIt->name.GetString()] = dictIt->value.GetInt();
      }
      primitive->targets.push_back(targetAttribues);
    }
  }

  ParseExtrasProperty(&(primitive->extras), o);

  ParseExtensionsProperty(&primitive->extensions, err, o);

#ifdef TINYGLTF_ENABLE_DRACO
  auto dracoExtension =
      primitive->extensions.find("KHR_draco_mesh_compression");
  if (dracoExtension != primitive->extensions.end()) {
    ParseDracoExtension(primitive, model, err, dracoExtension->second);
  }
#else
  (void)model;
#endif

  return true;
}

static bool ParseMesh(Mesh *mesh, Model *model, ptl::string *err,
                      const json::Value &o) {
  ParseStringProperty(&mesh->name, err, o, "name", false);

  mesh->primitives.clear();
  json::const_iterator primObject = o.FindMember("primitives");
  if ((primObject != o.MemberEnd()) && primObject->value.IsArray()) {
    for (auto i = primObject->value.Begin(); i != primObject->value.End();
         i++) {
      Primitive primitive;
      if (ParsePrimitive(&primitive, model, err, *i)) {
        
        mesh->primitives.push_back(primitive);
      }
    }
  }

  
  json::const_iterator targetsObject = o.FindMember("targets");
  if ((targetsObject != o.MemberEnd()) && targetsObject->value.IsArray()) {
    for (auto i = targetsObject->value.Begin(); i != targetsObject->value.End();
         i++) {
      ptl::map<ptl::string, int> targetAttribues;

      const json::Value &dict = *i;
      json::const_iterator dictIt(dict.MemberBegin());
      json::const_iterator dictItEnd(dict.MemberEnd());

      for (; dictIt != dictItEnd; ++dictIt) {
        targetAttribues[dictIt->name.GetString()] = dictIt->value.GetInt();
      }
      mesh->targets.push_back(targetAttribues);
    }
  }

  
  ParseNumberArrayProperty(&mesh->weights, err, o, "weights", false);

  ParseExtensionsProperty(&mesh->extensions, err, o);
  ParseExtrasProperty(&(mesh->extras), o);

  return true;
}

static bool ParseLight(Light *light, ptl::string *err, const json::Value &o) {
  ParseStringProperty(&light->name, err, o, "name", false);
  ParseNumberArrayProperty(&light->color, err, o, "color", false);
  ParseNumberProperty(&light->intensity, err, o, "intensity", false);
  ParseStringProperty(&light->type, err, o, "type", false);
  ParseNumberProperty(&light->distance, err, o, "distance", false);
  
  if(o.HasMember("spot"))
  {
      const json::Value& object = o["spot"];
      ParseNumberProperty(&light->innerConeAngle, err, object, "innerConeAngle", false);
      ParseNumberProperty(&light->outerConeAngle, err, object, "outerConeAngle", false);
  }
  return true;
}

static bool ParseNode(Node *node, ptl::string *err, const json::Value &o) {
  ParseStringProperty(&node->name, err, o, "name", false);

  double skin = -1.0;
  ParseNumberProperty(&skin, err, o, "skin", false);
  node->skin = static_cast<int>(skin);

  
  if (!ParseNumberArrayProperty(&node->matrix, err, o, "matrix", false)) {
    ParseNumberArrayProperty(&node->rotation, err, o, "rotation", false);
    ParseNumberArrayProperty(&node->scale, err, o, "scale", false);
    ParseNumberArrayProperty(&node->translation, err, o, "translation", false);
  }

  double camera = -1.0;
  ParseNumberProperty(&camera, err, o, "camera", false);
  node->camera = static_cast<int>(camera);

  double mesh = -1.0;
  ParseNumberProperty(&mesh, err, o, "mesh", false);
  node->mesh = int(mesh);

  node->children.clear();
  json::const_iterator childrenObject = o.FindMember("children");
  if ((childrenObject != o.MemberEnd()) && childrenObject->value.IsArray()) {
    for (auto i = childrenObject->value.Begin();
         i != childrenObject->value.End(); i++) {
      if (!i->IsNumber()) {
        if (err) {
          (*err) += "Invalid `children` array.\n";
        }
        return false;
      }
      const int &childrenNode = i->GetInt();
      node->children.push_back(childrenNode);
    }
  }

  ParseExtensionsProperty(&node->extensions, err, o);
  ParseExtrasProperty(&(node->extras), o);

  return true;
}

static bool ParseMaterial(Material *material, ptl::string *err,
                          const json::Value &o) {
  material->values.clear();
  material->extensions.clear();
  material->additionalValues.clear();

  json::const_iterator it(o.MemberBegin());
  json::const_iterator itEnd(o.MemberEnd());

  for (; it != itEnd; it++) {
    if (it->name.GetString() == ptl::string{"pbrMetallicRoughness"}) {
      if (it->value.IsObject()) {
        const json::Value &values_object = it->value;

        json::const_iterator itVal(values_object.MemberBegin());
        json::const_iterator itValEnd(values_object.MemberEnd());

        for (; itVal != itValEnd; itVal++) {
          Parameter param;
          if (ParseParameterProperty(&param, err, values_object,
                                     itVal->name.GetString(), false)) {
            material->values[itVal->name.GetString()] = param;
          }
        }
      }
    } else if (it->name.GetString() == ptl::string{"extensions"} ||
               it->name.GetString() == ptl::string{"extras"}) {
      
      
    } else {
      Parameter param;
      if (ParseParameterProperty(&param, err, o, it->name.GetString(), false)) {
        material->additionalValues[it->name.GetString()] = param;
      }
    }
  }

  ParseExtensionsProperty(&material->extensions, err, o);
  ParseExtrasProperty(&(material->extras), o);

  return true;
}

static bool ParseAnimationChannel(AnimationChannel *channel, ptl::string *err,
                                  const json::Value &o) {
  double samplerIndex = -1.0;
  double targetIndex = -1.0;
  if (!ParseNumberProperty(&samplerIndex, err, o, "sampler", true,
                           "AnimationChannel")) {
    if (err) {
      (*err) += "`sampler` field is missing in animation channels\n";
    }
    return false;
  }

  json::const_iterator targetIt = o.FindMember("target");
  if ((targetIt != o.MemberEnd()) && targetIt->value.IsObject()) {
    const json::Value &target_object = targetIt->value;

    if (!ParseNumberProperty(&targetIndex, err, target_object, "node", true)) {
      if (err) {
        (*err) += "`node` field is missing in animation.channels.target\n";
      }
      return false;
    }

    if (!ParseStringProperty(&channel->target_path, err, target_object, "path",
                             true)) {
      if (err) {
        (*err) += "`path` field is missing in animation.channels.target\n";
      }
      return false;
    }
  }

  channel->sampler = static_cast<int>(samplerIndex);
  channel->target_node = static_cast<int>(targetIndex);

  ParseExtrasProperty(&(channel->extras), o);

  return true;
}

static bool ParseAnimation(Animation *animation, ptl::string *err,
                           const json::Value &o) {
  {
    json::const_iterator channelsIt = o.FindMember("channels");
    if ((channelsIt != o.MemberEnd()) && channelsIt->value.IsArray()) {
      for (auto i = channelsIt->value.Begin();
           i != channelsIt->value.End(); i++) {
        AnimationChannel channel;
        if (ParseAnimationChannel(&channel, err, *i)) {
          
          animation->channels.push_back(channel);
        }
      }
    }
  }

  {
    json::const_iterator samplerIt = o.FindMember("samplers");
    if ((samplerIt != o.MemberEnd()) && samplerIt->value.IsArray()) {
      const json::Value &sampler_array = samplerIt->value;

      json::Value::ConstValueIterator it = sampler_array.Begin();
      json::Value::ConstValueIterator itEnd = sampler_array.End();

      for (; it != itEnd; it++) {
        const json::Value &s = *it;

        AnimationSampler sampler;
        double inputIndex = -1.0;
        double outputIndex = -1.0;
        if (!ParseNumberProperty(&inputIndex, err, s, "input", true)) {
          if (err) {
            (*err) += "`input` field is missing in animation.sampler\n";
          }
          return false;
        }
        ParseStringProperty(&sampler.interpolation, err, s, "interpolation",
                            false);
        if (!ParseNumberProperty(&outputIndex, err, s, "output", true)) {
          if (err) {
            (*err) += "`output` field is missing in animation.sampler\n";
          }
          return false;
        }
        sampler.input = static_cast<int>(inputIndex);
        sampler.output = static_cast<int>(outputIndex);
        ParseExtrasProperty(&(sampler.extras), s);
        animation->samplers.push_back(sampler);
      }
    }
  }

  ParseStringProperty(&animation->name, err, o, "name", false);

  ParseExtrasProperty(&(animation->extras), o);

  return true;
}

static bool ParseSampler(Sampler *sampler, ptl::string *err,
                         const json::Value &o) {
  ParseStringProperty(&sampler->name, err, o, "name", false);

  double minFilter =
      static_cast<double>(TINYGLTF_TEXTURE_FILTER_NEAREST_MIPMAP_LINEAR);
  double magFilter = static_cast<double>(TINYGLTF_TEXTURE_FILTER_LINEAR);
  double wrapS = static_cast<double>(TINYGLTF_TEXTURE_WRAP_REPEAT);
  double wrapT = static_cast<double>(TINYGLTF_TEXTURE_WRAP_REPEAT);
  ParseNumberProperty(&minFilter, err, o, "minFilter", false);
  ParseNumberProperty(&magFilter, err, o, "magFilter", false);
  ParseNumberProperty(&wrapS, err, o, "wrapS", false);
  ParseNumberProperty(&wrapT, err, o, "wrapT", false);

  sampler->minFilter = static_cast<int>(minFilter);
  sampler->magFilter = static_cast<int>(magFilter);
  sampler->wrapS = static_cast<int>(wrapS);
  sampler->wrapT = static_cast<int>(wrapT);

  ParseExtrasProperty(&(sampler->extras), o);

  return true;
}

static bool ParseSkin(Skin *skin, ptl::string *err, const json::Value &o) {
  ParseStringProperty(&skin->name, err, o, "name", false, "Skin");

  ptl::vector<double> joints;
  if (!ParseNumberArrayProperty(&joints, err, o, "joints", false, "Skin")) {
    return false;
  }

  double skeleton = -1.0;
  ParseNumberProperty(&skeleton, err, o, "skeleton", false, "Skin");
  skin->skeleton = static_cast<int>(skeleton);

  skin->joints.resize(joints.size());
  for (size_t i = 0; i < joints.size(); i++) {
    skin->joints[i] = static_cast<int>(joints[i]);
  }

  double invBind = -1.0;
  ParseNumberProperty(&invBind, err, o, "inverseBindMatrices", true, "Skin");
  skin->inverseBindMatrices = static_cast<int>(invBind);

  return true;
}

static bool ParsePerspectiveCamera(PerspectiveCamera *camera, ptl::string *err,
                                   const json::Value &o) {
  double yfov = 0.0;
  if (!ParseNumberProperty(&yfov, err, o, "yfov", true, "OrthographicCamera")) {
    return false;
  }

  double znear = 0.0;
  if (!ParseNumberProperty(&znear, err, o, "znear", true,
                           "PerspectiveCamera")) {
    return false;
  }

  double aspectRatio = 0.0;  
  ParseNumberProperty(&aspectRatio, err, o, "aspectRatio", false,
                      "PerspectiveCamera");

  double zfar = 0.0;  
  ParseNumberProperty(&zfar, err, o, "zfar", false, "PerspectiveCamera");

  camera->aspectRatio = aspectRatio;
  camera->zfar = zfar;
  camera->yfov = yfov;
  camera->znear = znear;

  ParseExtensionsProperty(&camera->extensions, err, o);
  ParseExtrasProperty(&(camera->extras), o);

  

  return true;
}

static bool ParseOrthographicCamera(OrthographicCamera *camera,
                                    ptl::string *err, const json::Value &o) {
  double xmag = 0.0;
  if (!ParseNumberProperty(&xmag, err, o, "xmag", true, "OrthographicCamera")) {
    return false;
  }

  double ymag = 0.0;
  if (!ParseNumberProperty(&ymag, err, o, "ymag", true, "OrthographicCamera")) {
    return false;
  }

  double zfar = 0.0;
  if (!ParseNumberProperty(&zfar, err, o, "zfar", true, "OrthographicCamera")) {
    return false;
  }

  double znear = 0.0;
  if (!ParseNumberProperty(&znear, err, o, "znear", true,
                           "OrthographicCamera")) {
    return false;
  }

  ParseExtensionsProperty(&camera->extensions, err, o);
  ParseExtrasProperty(&(camera->extras), o);

  camera->xmag = xmag;
  camera->ymag = ymag;
  camera->zfar = zfar;
  camera->znear = znear;

  

  return true;
}

static bool ParseCamera(Camera *camera, ptl::string *err,
                        const json::Value &o) {
  if (!ParseStringProperty(&camera->type, err, o, "type", true, "Camera")) {
    return false;
  }

  if (camera->type.compare("orthographic") == 0) {
    if (o.FindMember("orthographic") == o.MemberEnd()) {
      if (err) {
        ptl::stringstream ss;
        ss << "Orhographic camera description not found." << std::endl;
        (*err) += ss.str();
      }
      return false;
    }

    const json::Value &v = o.FindMember("orthographic")->value;
    if (!v.IsObject()) {
      if (err) {
        ptl::stringstream ss;
        ss << "\"orthographic\" is not a JSON object." << std::endl;
        (*err) += ss.str();
      }
      return false;
    }

    if (!ParseOrthographicCamera(&camera->orthographic, err, v)) {
      return false;
    }
  } else if (camera->type.compare("perspective") == 0) {
    if (o.FindMember("perspective") == o.MemberEnd()) {
      if (err) {
        ptl::stringstream ss;
        ss << "Perspective camera description not found." << std::endl;
        (*err) += ss.str();
      }
      return false;
    }

    const json::Value &v = o.FindMember("perspective")->value;
    if (!v.IsObject()) {
      if (err) {
        ptl::stringstream ss;
        ss << "\"perspective\" is not a JSON object." << std::endl;
        (*err) += ss.str();
      }
      return false;
    }

    if (!ParsePerspectiveCamera(&camera->perspective, err, v)) {
      return false;
    }
  } else {
    if (err) {
      ptl::stringstream ss;
      ss << "Invalid camera type: \"" << camera->type
         << "\". Must be \"perspective\" or \"orthographic\"" << std::endl;
      (*err) += ss.str();
    }
    return false;
  }

  ParseStringProperty(&camera->name, err, o, "name", false);

  ParseExtensionsProperty(&camera->extensions, err, o);
  ParseExtrasProperty(&(camera->extras), o);

  return true;
}

bool TinyGLTF::LoadFromString(Model *model, ptl::string *err, ptl::string *warn,
                              const char *str, unsigned int length,
                              const ptl::string &base_dir,
                              unsigned int check_sections) {
  if (length < 4) {
    if (err) {
      (*err) = "JSON string too short.\n";
    }
    return false;
  }

  json::Value v;
  json::Doc document;

#if (defined(__cpp_exceptions) || defined(__EXCEPTIONS) || defined(_CPPUNWIND)) && not defined(TINYGLTF_NOEXCEPTION)
  try {
    v = json::parse(str, str + length);

  } catch (const std::exception &e) {
    if (err) {
      (*err) = e.what();
    }
    return false;
  }
#else
  {
	  auto& test = document.GetAllocator();
    document.Parse(str, length);
    v.CopyFrom(document, document.GetAllocator());

    if (!v.IsObject()) {
      
      if (err) {
        (*err) = "Failed to parse JSON object\n";
      }
      return false;
    }
  }
#endif

  if (!v.IsObject()) {
    
    if (err) {
      (*err) = "Root element is not a JSON object\n";
    }
    return false;
  }

  
  

  {
    json::const_iterator it = v.FindMember("scenes");
    if ((it != v.MemberEnd()) && it->value.IsArray()) {
      
    } else if (check_sections & REQUIRE_SCENES) {
      if (err) {
        (*err) += "\"scenes\" object not found in .gltf or not an array type\n";
      }
      return false;
    }
  }

  {
    json::const_iterator it = v.FindMember("nodes");
    if ((it != v.MemberEnd()) && it->value.IsArray()) {
      
    } else if (check_sections & REQUIRE_NODES) {
      if (err) {
        (*err) += "\"nodes\" object not found in .gltf\n";
      }
      return false;
    }
  }

  {
    json::const_iterator it = v.FindMember("accessors");
    if ((it != v.MemberEnd()) && it->value.IsArray()) {
      
    } else if (check_sections & REQUIRE_ACCESSORS) {
      if (err) {
        (*err) += "\"accessors\" object not found in .gltf\n";
      }
      return false;
    }
  }

  {
    json::const_iterator it = v.FindMember("buffers");
    if ((it != v.MemberEnd()) && it->value.IsArray()) {
      
    } else if (check_sections & REQUIRE_BUFFERS) {
      if (err) {
        (*err) += "\"buffers\" object not found in .gltf\n";
      }
      return false;
    }
  }

  {
    json::const_iterator it = v.FindMember("bufferViews");
    if ((it != v.MemberEnd()) && it->value.IsArray()) {
      
    } else if (check_sections & REQUIRE_BUFFER_VIEWS) {
      if (err) {
        (*err) += "\"bufferViews\" object not found in .gltf\n";
      }
      return false;
    }
  }

  model->buffers.clear();
  model->bufferViews.clear();
  model->accessors.clear();
  model->meshes.clear();
  model->cameras.clear();
  model->nodes.clear();
  model->extensionsUsed.clear();
  model->extensionsRequired.clear();
  model->extensions.clear();
  model->defaultScene = -1;

  
  {
    json::const_iterator it = v.FindMember("asset");
    if ((it != v.MemberEnd()) && it->value.IsObject()) {
      const json::Value &root = it->value;

      ParseAsset(&model->asset, err, root);
    }
  }

  
  {
    json::const_iterator it = v.FindMember("extensionsUsed");
    if ((it != v.MemberEnd()) && it->value.IsArray()) {
      const json::Value &root = it->value;
      for (unsigned int i = 0; i < root.Size(); ++i) {
        model->extensionsUsed.push_back(root[i].GetString());
      }
    }
  }

  {
    json::const_iterator it = v.FindMember("extensionsRequired");
    if ((it != v.MemberEnd()) && it->value.IsArray()) {
      const json::Value &root = it->value;
      for (unsigned int i = 0; i < root.Size(); ++i) {
        model->extensionsRequired.push_back(root[i].GetString());
      }
    }
  }

  
  {
    json::const_iterator rootIt = v.FindMember("buffers");
    if ((rootIt != v.MemberEnd()) && rootIt->value.IsArray()) {
      const json::Value &root = rootIt->value;

      json::Value::ConstValueIterator it(root.Begin());
      json::Value::ConstValueIterator itEnd(root.End());
      for (; it != itEnd; it++) {
        if (!it->IsObject()) {
          if (err) {
            (*err) += "`buffers' does not contain an JSON object.";
          }
          return false;
        }
        Buffer buffer;
        if (!ParseBuffer(&buffer, err, *it, &fs, base_dir, is_binary_,
                         bin_data_, bin_size_)) {
          return false;
        }

        model->buffers.push_back(buffer);
      }
    }
  }

  
  {
    json::const_iterator rootIt = v.FindMember("bufferViews");
    if ((rootIt != v.MemberEnd()) && rootIt->value.IsArray()) {
      const json::Value &root = rootIt->value;

      auto it(root.Begin());
      auto itEnd(root.End());
      for (; it != itEnd; it++) {
        if (!it->IsObject()) {
          if (err) {
            (*err) += "`bufferViews' does not contain an JSON object.";
          }
          return false;
        }
        BufferView bufferView;
        if (!ParseBufferView(&bufferView, err, *it)) {
          return false;
        }

        model->bufferViews.push_back(bufferView);
      }
    }
  }

  
  {
    json::const_iterator rootIt = v.FindMember("accessors");
    if ((rootIt != v.MemberEnd()) && rootIt->value.IsArray()) {
      const json::Value &root = rootIt->value;

      auto it(root.Begin());
      auto itEnd(root.End());
      for (; it != itEnd; it++) {
        if (!it->IsObject()) {
          if (err) {
            (*err) += "`accessors' does not contain an JSON object.";
          }
          return false;
        }
        Accessor accessor;
        if (!ParseAccessor(&accessor, err, *it)) {
          return false;
        }

        model->accessors.push_back(accessor);
      }
    }
  }

  
  {
    json::const_iterator rootIt = v.FindMember("meshes");
    if ((rootIt != v.MemberEnd()) && rootIt->value.IsArray()) {
      const json::Value &root = rootIt->value;

      auto it(root.Begin());
      auto itEnd(root.End());
      for (; it != itEnd; it++) {
        if (!it->IsObject()) {
          if (err) {
            (*err) += "`meshes' does not contain an JSON object.";
          }
          return false;
        }
        Mesh mesh;
        if (!ParseMesh(&mesh, model, err, *it)) {
          return false;
        }

        model->meshes.push_back(mesh);
      }
    }
  }

  
  
  
  for (auto &mesh : model->meshes) {
    for (auto &primitive : mesh.primitives) {
      if (primitive.indices >
          -1)  
      {
        model->bufferViews[model->accessors[primitive.indices].bufferView]
            .target = TINYGLTF_TARGET_ELEMENT_ARRAY_BUFFER;
        
        
      }
    }
  }
  
  
  for (auto &bufferView : model->bufferViews) {
    if (bufferView.target == 0)  
    {
      bufferView.target = TINYGLTF_TARGET_ARRAY_BUFFER;
    }
  }

  
  {
    json::const_iterator rootIt = v.FindMember("nodes");
    if ((rootIt != v.MemberEnd()) && rootIt->value.IsArray()) {
      const json::Value &root = rootIt->value;

      auto it(root.Begin());
      auto itEnd(root.End());
      for (; it != itEnd; it++) {
        if (!it->IsObject()) {
          if (err) {
            (*err) += "`nodes' does not contain an JSON object.";
          }
          return false;
        }
        Node node;
        if (!ParseNode(&node, err, *it)) {
          return false;
        }

        model->nodes.push_back(node);
      }
    }
  }

  
  {
    json::const_iterator rootIt = v.FindMember("scenes");
    if ((rootIt != v.MemberEnd()) && rootIt->value.IsArray()) {
      const json::Value &root = rootIt->value;

      auto it(root.Begin());
      auto itEnd(root.End());
      for (; it != itEnd; it++) {
        if (!(it->IsObject())) {
          if (err) {
            (*err) += "`scenes' does not contain an JSON object.";
          }
          return false;
        }
        const json::Value &o = *it;
        ptl::vector<double> nodes;
        if (!ParseNumberArrayProperty(&nodes, err, o, "nodes", false)) {
          return false;
        }

        Scene scene;
        ParseStringProperty(&scene.name, err, o, "name", false);
        ptl::vector<int> nodesIds;
        for (size_t i = 0; i < nodes.size(); i++) {
          nodesIds.push_back(static_cast<int>(nodes[i]));
        }
        scene.nodes = nodesIds;

        ParseExtensionsProperty(&scene.extensions, err, o);
        ParseExtrasProperty(&scene.extras, o);

        model->scenes.push_back(scene);
      }
    }
  }

  
  {
    json::const_iterator rootIt = v.FindMember("scene");
    if ((rootIt != v.MemberEnd()) && rootIt->value.IsNumber()) {
      const int defaultScene = rootIt->value.GetInt();

      model->defaultScene = static_cast<int>(defaultScene);
    }
  }

  
  {
    json::const_iterator rootIt = v.FindMember("materials");
    if ((rootIt != v.MemberEnd()) && rootIt->value.IsArray()) {
      const json::Value &root = rootIt->value;

      auto it(root.Begin());
      auto itEnd(root.End());
      for (; it != itEnd; it++) {
        if (!it->IsObject()) {
          if (err) {
            (*err) += "`materials' does not contain an JSON object.";
          }
          return false;
        }
        json::Value jsonMaterial;
        jsonMaterial.CopyFrom(*it, document.GetAllocator());

        Material material;
        ParseStringProperty(&material.name, err, jsonMaterial, "name", false);

        if (!ParseMaterial(&material, err, jsonMaterial)) {
          return false;
        }

        model->materials.push_back(material);
      }
    }
  }

  
  {
    json::const_iterator rootIt = v.FindMember("images");
    if ((rootIt != v.MemberEnd()) && rootIt->value.IsArray()) {
      const json::Value &root = rootIt->value;

      auto it(root.Begin());
      auto itEnd(root.End());
      int idx = 0;
      for (; it != itEnd; it++, idx++) {
        if (!it->IsObject()) {
          if (err) {
            (*err) +=
                "image[" + ptl::to_string(idx) + "] is not a JSON object.";
          }
          return false;
        }
        Image image;
        if (!ParseImage(&image, idx, err, warn, *it, base_dir, &fs,
                        &this->LoadImageData, load_image_user_data_)) {
          return false;
        }

        if (image.bufferView != -1) {
          
          if (size_t(image.bufferView) >= model->bufferViews.size()) {
            if (err) {
              ptl::stringstream ss;
              ss << "image[" << idx << "] bufferView \"" << image.bufferView
                 << "\" not found in the scene." << std::endl;
              (*err) += ss.str();
            }
            return false;
          }
          
          const BufferView &bufferView =
              model->bufferViews[size_t(image.bufferView)];
          const Buffer &buffer = model->buffers[size_t(bufferView.buffer)];

          if (*LoadImageData == nullptr) {
            if (err) {
              (*err) += "No LoadImageData callback specified.\n";
            }
            return false;
          }

          bool ret = LoadImageData(
              &image, idx, err, warn, image.width, image.height,
              &buffer.data[bufferView.byteOffset],
              static_cast<int>(bufferView.byteLength), load_image_user_data_);
          if (!ret) {
            return false;
          }
        }

        model->images.push_back(image);
      }
    }
  }

  
  {
    json::const_iterator rootIt = v.FindMember("textures");
    if ((rootIt != v.MemberEnd()) && rootIt->value.IsArray()) {
      const json::Value &root = rootIt->value;

      auto it(root.Begin());
      auto itEnd(root.End());
      for (; it != itEnd; it++) {
        if (!it->IsObject()) {
          if (err) {
            (*err) += "`textures' does not contain an JSON object.";
          }
          return false;
        }
        Texture texture;
        if (!ParseTexture(&texture, err, *it, base_dir)) {
          return false;
        }
        model->textures.push_back(texture);
      }
    }
  }

  
  {
    json::const_iterator rootIt = v.FindMember("animations");
    if ((rootIt != v.MemberEnd()) && rootIt->value.IsArray()) {
      const json::Value &root = rootIt->value;

      auto it(root.Begin());
      auto itEnd(root.End());
      for (; it != itEnd; ++it) {
        if (!it->IsObject()) {
          if (err) {
            (*err) += "`animations' does not contain an JSON object.";
          }
          return false;
        }
        Animation animation;
        if (!ParseAnimation(&animation, err, *it)) {
          return false;
        }

        model->animations.push_back(animation);
      }
    }
  }

  
  {
    json::const_iterator rootIt = v.FindMember("skins");
    if ((rootIt != v.MemberEnd()) && rootIt->value.IsArray()) {
      const json::Value &root = rootIt->value;

      auto it(root.Begin());
      auto itEnd(root.End());
      for (; it != itEnd; ++it) {
        if (!it->IsObject()) {
          if (err) {
            (*err) += "`skins' does not contain an JSON object.";
          }
          return false;
        }
        Skin skin;
        if (!ParseSkin(&skin, err, *it)) {
          return false;
        }

        model->skins.push_back(skin);
      }
    }
  }

  
  {
    json::const_iterator rootIt = v.FindMember("samplers");
    if ((rootIt != v.MemberEnd()) && rootIt->value.IsArray()) {
      const json::Value &root = rootIt->value;

      auto it(root.Begin());
      auto itEnd(root.End());
      for (; it != itEnd; ++it) {
        if (!it->IsObject()) {
          if (err) {
            (*err) += "`samplers' does not contain an JSON object.";
          }
          return false;
        }
        Sampler sampler;
        if (!ParseSampler(&sampler, err, *it)) {
          return false;
        }

        model->samplers.push_back(sampler);
      }
    }
  }

  
  {
    json::const_iterator rootIt = v.FindMember("cameras");
    if ((rootIt != v.MemberEnd()) && rootIt->value.IsArray()) {
      const json::Value &root = rootIt->value;

      auto it(root.End());
      auto itEnd(root.End());
      for (; it != itEnd; ++it) {
        if (!it->IsObject()) {
          if (err) {
            (*err) += "`cameras' does not contain an JSON object.";
          }
          return false;
        }
        Camera camera;
        if (!ParseCamera(&camera, err, *it)) {
          return false;
        }

        model->cameras.push_back(camera);
      }
    }
  }

  
  ParseExtensionsProperty(&model->extensions, err, v);

  
  {
    json::const_iterator rootIt = v.FindMember("extensions");
    if ((rootIt != v.MemberEnd()) && rootIt->value.IsObject()) {
      const json::Value &root = rootIt->value;

      auto it(root.MemberBegin());
      auto itEnd(root.MemberEnd());
      for (; it != itEnd; ++it) {
        
        if ((ptl::string{it->name.GetString()}.compare("KHR_lights_punctual") == 0) &&
            it->value.IsObject()) {
          const json::Value &object = it->value;
          json::const_iterator itLight(object.FindMember("lights"));
          json::const_iterator itLightEnd(object.MemberEnd());
          if (itLight == itLightEnd) {
            continue;
          }

          if (!itLight->value.IsArray()) {
            continue;
          }

          const json::Value &lights = itLight->value;
          auto arrayIt(lights.Begin());
          auto arrayItEnd(lights.End());
          for (; arrayIt != arrayItEnd; ++arrayIt) {
            Light light;
            if (!ParseLight(&light, err, *arrayIt)) {
              return false;
            }
            model->lights.push_back(light);
          }
        }
      }
    }
  }

  
  ParseExtrasProperty(&model->extras, v);

  return true;
}

bool TinyGLTF::LoadASCIIFromString(Model *model, ptl::string *err,
                                   ptl::string *warn, const char *str,
                                   unsigned int length,
                                   const ptl::string &base_dir,
                                   unsigned int check_sections) {
  is_binary_ = false;
  bin_data_ = nullptr;
  bin_size_ = 0;

  return LoadFromString(model, err, warn, str, length, base_dir,
                        check_sections);
}

bool TinyGLTF::LoadASCIIFromFile(Model *model, ptl::string *err,
                                 ptl::string *warn, const ptl::string &filename,
                                 unsigned int check_sections) {
  ptl::stringstream ss;

  if (fs.ReadWholeFile == nullptr) {
    
    ss << "Failed to read file: " << filename
       << ": one or more FS callback not set" << std::endl;
    if (err) {
      (*err) = ss.str();
    }
    return false;
  }

  ptl::vector<unsigned char> data;
  ptl::string fileerr;
  bool fileread = fs.ReadWholeFile(&data, &fileerr, filename, fs.user_data);
  if (!fileread) {
    ss << "Failed to read file: " << filename << ": " << fileerr << std::endl;
    if (err) {
      (*err) = ss.str();
    }
    return false;
  }

  size_t sz = data.size();
  if (sz == 0) {
    if (err) {
      (*err) = "Empty file.";
    }
    return false;
  }

  ptl::string basedir = GetBaseDir(filename);

  bool ret = LoadASCIIFromString(
      model, err, warn, reinterpret_cast<const char *>(&data.at(0)),
      static_cast<unsigned int>(data.size()), basedir, check_sections);

  return ret;
}

bool TinyGLTF::LoadBinaryFromMemory(Model *model, ptl::string *err,
                                    ptl::string *warn,
                                    const unsigned char *bytes,
                                    unsigned int size,
                                    const ptl::string &base_dir,
                                    unsigned int check_sections) {
  if (size < 20) {
    if (err) {
      (*err) = "Too short data size for glTF Binary.";
    }
    return false;
  }

  if (bytes[0] == 'g' && bytes[1] == 'l' && bytes[2] == 'T' &&
      bytes[3] == 'F') {
    
  } else {
    if (err) {
      (*err) = "Invalid magic.";
    }
    return false;
  }

  unsigned int version;       
  unsigned int length;        
  unsigned int model_length;  
  unsigned int model_format;  

  
  memcpy(&version, bytes + 4, 4);
  swap4(&version);
  memcpy(&length, bytes + 8, 4);
  swap4(&length);
  memcpy(&model_length, bytes + 12, 4);
  swap4(&model_length);
  memcpy(&model_format, bytes + 16, 4);
  swap4(&model_format);

  
  
  
  if ((20 + model_length > size) || (model_length < 1) ||
      (model_format != 0x4E4F534A)) {  
    if (err) {
      (*err) = "Invalid glTF binary.";
    }
    return false;
  }

  
  ptl::string jsonString(reinterpret_cast<const char *>(&bytes[20]),
                         model_length);

  is_binary_ = true;
  bin_data_ = bytes + 20 + model_length +
              8;  
  bin_size_ =
      length - (20 + model_length);  

  bool ret = LoadFromString(model, err, warn,
                            reinterpret_cast<const char *>(&bytes[20]),
                            model_length, base_dir, check_sections);
  if (!ret) {
    return ret;
  }

  return true;
}

bool TinyGLTF::LoadBinaryFromFile(Model *model, ptl::string *err,
                                  ptl::string *warn,
                                  const ptl::string &filename,
                                  unsigned int check_sections) {
  ptl::stringstream ss;

  if (fs.ReadWholeFile == nullptr) {
    
    ss << "Failed to read file: " << filename
       << ": one or more FS callback not set" << std::endl;
    if (err) {
      (*err) = ss.str();
    }
    return false;
  }

  ptl::vector<unsigned char> data;
  ptl::string fileerr;
  bool fileread = fs.ReadWholeFile(&data, &fileerr, filename, fs.user_data);
  if (!fileread) {
    ss << "Failed to read file: " << filename << ": " << fileerr << std::endl;
    if (err) {
      (*err) = ss.str();
    }
    return false;
  }

  ptl::string basedir = GetBaseDir(filename);

  bool ret = LoadBinaryFromMemory(model, err, warn, &data.at(0),
                                  static_cast<unsigned int>(data.size()),
                                  basedir, check_sections);

  return ret;
}







template <typename T>
static void SerializeNumberProperty(const ptl::string &key, T number,
                                    json::Value &obj,
                                    json::Doc::AllocatorType &allocator) {
  
  
  
	json::Value tempVal{ key.c_str(), allocator };
  obj.AddMember(tempVal, number, allocator);
}

template <typename T>
static void SerializeNumberArrayProperty(const ptl::string &key,
                                         const ptl::vector<T> &value,
                                         json::Value &obj,
                                         json::Doc::AllocatorType &allocator) {
  json::Value vals{rapidjson::Type::kArrayType};

  for (unsigned int i = 0; i < value.size(); ++i) {
    vals.PushBack(static_cast<T>(value[i]), allocator);
  }
  if (!vals.IsNull()) {
	  json::Value nameValue { key.c_str(), allocator };
    obj.AddMember(nameValue, vals, allocator);
  }
}

static void SerializeStringProperty(const ptl::string &key,
                                    const ptl::string &value, json::Value &obj,
                                    json::Doc::AllocatorType &allocator) {
	json::Value nameValue{ key.c_str(), allocator };
	json::Value valueValue{ value.c_str(), allocator };
  obj.AddMember(nameValue, valueValue, allocator);
}

static void SerializeStringArrayProperty(const ptl::string &key,
                                         const ptl::vector<ptl::string> &value,
                                         json::Value &obj,
                                         json::Doc::AllocatorType &allocator) {
  
  json::Value vals{rapidjson::Type::kArrayType};

  for (unsigned int i = 0; i < value.size(); ++i) {
	  json::Value tempVal{ value[i].c_str(), allocator };
    vals.PushBack(tempVal, allocator);
  }
  json::Value tempVal{ key.c_str(), allocator };
  obj.AddMember(tempVal, vals,
                allocator);
}

static bool ValueToJson(const Value &value, json::Value *ret,
                        json::Doc::AllocatorType &allocator) {
  json::Value obj;
  switch (value.Type()) {
  case NUMBER_TYPE: {
	  json::Value doubleTempVal{ value.Get<double>() };
	  obj = doubleTempVal;
	  break;
  }
  case INT_TYPE: {
	  json::Value intTempVal(value.Get<int>());
	  obj = intTempVal;
	  break;
  }
  case BOOL_TYPE: {
	  json::Value boolTempVal(value.Get<bool>());
	  obj = boolTempVal;
	  break;
  }
  case STRING_TYPE: {
	  json::Value stringTempVal{ value.Get<ptl::string>().c_str(), allocator };
	  obj = stringTempVal;
	  break;
  }
    case ARRAY_TYPE: {
      for (unsigned int i = 0; i < value.ArrayLen(); ++i) {
        Value elementValue = value.Get(int(i));
        json::Value elementJson;
        if (ValueToJson(value.Get(int(i)), &elementJson, allocator))
          obj.PushBack(elementJson, allocator);
      }
      break;
    }
	case BINARY_TYPE: {
		
		
		return false;
		break;
	}
    case OBJECT_TYPE: {
      Value::Object objMap = value.Get<Value::Object>();
      for (auto &it : objMap) {
        json::Value elementJson;
		if (ValueToJson(it.second, &elementJson, allocator)) {
			json::Value tmpNameVal{ it.first.c_str(), allocator };
			obj.AddMember(tmpNameVal, elementJson,
				allocator);
		}
      }
      break;
    }
    case NULL_TYPE:
    default:
      return false;
  }
  if (ret) *ret = obj;
  return true;
}

static void SerializeValue(const ptl::string &key, const Value &value,
                           json::Value &obj,
                           json::Doc::AllocatorType &allocator) {
  json::Value ret;
  if (ValueToJson(value, &ret, allocator)) {
	  json::Value nameVal{ key.c_str(), allocator };
	  obj.AddMember(nameVal, ret, allocator);
  }
}

static void SerializeGltfBufferData(const ptl::vector<unsigned char> &data,
                                    json::Value &o,
                                    json::Doc::AllocatorType &allocator) {
  ptl::string header = "data:application/octet-stream;base64,";
  ptl::string encodedData =
      base64_encode(&data[0], static_cast<unsigned int>(data.size()));
  SerializeStringProperty("uri", header + encodedData, o, allocator);
}

static bool SerializeGltfBufferData(const ptl::vector<unsigned char> &data,
                                    const ptl::string &binFilename) {
	std::ofstream output{ binFilename.c_str(), std::ofstream::binary };
  if (!output.is_open()) return false;
  output.write(reinterpret_cast<const char *>(&data[0]),
               std::streamsize(data.size()));
  output.close();
  return true;
}

static void SerializeParameterMap(ParameterMap &param, json::Value &o,
                                  json::Doc::AllocatorType &allocator) {
  for (ParameterMap::iterator paramIt = param.begin(); paramIt != param.end();
       ++paramIt) {
    if (paramIt->second.number_array.size()) {
      SerializeNumberArrayProperty<double>(
          paramIt->first, paramIt->second.number_array, o, allocator);
    } else if (paramIt->second.json_double_value.size()) {
      json::Value json_double_value;
      for (ptl::map<ptl::string, double>::iterator it =
               paramIt->second.json_double_value.begin();
           it != paramIt->second.json_double_value.end(); ++it) {
        if (it->first == "index") {
			json::Value nameVal{ it->first.c_str(), allocator };
          json_double_value.AddMember(nameVal,
                                      paramIt->second.TextureIndex(),
                                      allocator);
        } else {
			json::Value nameVal{ it->first.c_str(), allocator };
          json_double_value.AddMember(nameVal,
                                      it->second, allocator);
        }
      }

	  json::Value tempNameValue{ paramIt->first.c_str(), allocator };
      o.AddMember(tempNameValue,
                  json_double_value, allocator);
    } else if (!paramIt->second.string_value.empty()) {
      SerializeStringProperty(paramIt->first, paramIt->second.string_value, o,
                              allocator);
    } else if (paramIt->second.has_number_value) {
		json::Value tmpNameVal{ paramIt->first.c_str(), allocator };
      o.AddMember(tmpNameVal,
                  paramIt->second.number_value, allocator);
    } else {
		json::Value tmpNameVal{ paramIt->first.c_str(), allocator };
      o.AddMember(tmpNameVal,
                  paramIt->second.bool_value, allocator);
    }
  }
}

static void SerializeExtensionMap(ExtensionMap &extensions, json::Value &o,
                                  json::Doc::AllocatorType &allocator) {
  if (!extensions.size()) return;

  json::Value extMap{rapidjson::Type::kObjectType};
  for (ExtensionMap::iterator extIt = extensions.begin();
       extIt != extensions.end(); ++extIt) {
    json::Value extension_values;

    
    json::Value ret;
    if (ValueToJson(extIt->second, &ret, allocator)) {
		json::Value nameVal{ extIt->first.c_str(), allocator };
      extMap.AddMember(nameVal, ret,
                       allocator);
    }
    if (ret.IsNull()) {
      if (!(extIt->first.empty())) {  
        
        
		  json::Value nameVal{ extIt->first.c_str(), allocator };
		  json::Value emptyVal{};
        extMap.AddMember(nameVal,
			emptyVal, allocator);
      }
    }
  }
  o.AddMember("extensions", extMap, allocator);
}

static void SerializeGltfAccessor(Accessor &accessor, json::Value &o,
                                  json::Doc::AllocatorType &allocator) {
  SerializeNumberProperty<int>("bufferView", accessor.bufferView, o, allocator);

  if (accessor.byteOffset != 0.0)
    SerializeNumberProperty<int>("byteOffset", int(accessor.byteOffset), o,
                                 allocator);

  SerializeNumberProperty<int>("componentType", accessor.componentType, o,
                               allocator);
  SerializeNumberProperty<size_t>("count", accessor.count, o, allocator);
  SerializeNumberArrayProperty<double>("min", accessor.minValues, o, allocator);
  SerializeNumberArrayProperty<double>("max", accessor.maxValues, o, allocator);
  ptl::string type;
  switch (accessor.type) {
    case TINYGLTF_TYPE_SCALAR:
      type = "SCALAR";
      break;
    case TINYGLTF_TYPE_VEC2:
      type = "VEC2";
      break;
    case TINYGLTF_TYPE_VEC3:
      type = "VEC3";
      break;
    case TINYGLTF_TYPE_VEC4:
      type = "VEC4";
      break;
    case TINYGLTF_TYPE_MAT2:
      type = "MAT2";
      break;
    case TINYGLTF_TYPE_MAT3:
      type = "MAT3";
      break;
    case TINYGLTF_TYPE_MAT4:
      type = "MAT4";
      break;
  }

  SerializeStringProperty("type", type, o, allocator);
  if (!accessor.name.empty())
    SerializeStringProperty("name", accessor.name, o, allocator);

  if (accessor.extras.Type() != NULL_TYPE) {
    SerializeValue("extras", accessor.extras, o, allocator);
  }
}

static void SerializeGltfAnimationChannel(AnimationChannel &channel,
                                          json::Value &o,
                                          json::Doc::AllocatorType &allocator) {
  SerializeNumberProperty("sampler", channel.sampler, o, allocator);
  json::Value target;
  SerializeNumberProperty("node", channel.target_node, target, allocator);
  SerializeStringProperty("path", channel.target_path, target, allocator);

  o["target"] = target;

  if (channel.extras.Type() != NULL_TYPE) {
    SerializeValue("extras", channel.extras, o, allocator);
  }
}

static void SerializeGltfAnimationSampler(AnimationSampler &sampler,
                                          json::Value &o,
                                          json::Doc::AllocatorType &allocator) {
  SerializeNumberProperty("input", sampler.input, o, allocator);
  SerializeNumberProperty("output", sampler.output, o, allocator);
  SerializeStringProperty("interpolation", sampler.interpolation, o, allocator);

  if (sampler.extras.Type() != NULL_TYPE) {
    SerializeValue("extras", sampler.extras, o, allocator);
  }
}

static void SerializeGltfAnimation(Animation &animation, json::Value &o,
                                   json::Doc::AllocatorType &allocator) {
  if (!animation.name.empty())
    SerializeStringProperty("name", animation.name, o, allocator);
  json::Value channels;
  for (unsigned int i = 0; i < animation.channels.size(); ++i) {
    json::Value channel;
    AnimationChannel gltfChannel = animation.channels[i];
    SerializeGltfAnimationChannel(gltfChannel, channel, allocator);
    channels.PushBack(channel, allocator);
  }
  o.AddMember("channels", channels, allocator);

  json::Value samplers;
  for (unsigned int i = 0; i < animation.samplers.size(); ++i) {
    json::Value sampler;
    AnimationSampler gltfSampler = animation.samplers[i];
    SerializeGltfAnimationSampler(gltfSampler, sampler, allocator);
    samplers.PushBack(sampler, allocator);
  }

  o.AddMember("samplers", samplers, allocator);

  if (animation.extras.Type() != NULL_TYPE) {
    SerializeValue("extras", animation.extras, o, allocator);
  }
}

static void SerializeGltfAsset(Asset &asset, json::Value &o,
                               json::Doc::AllocatorType &allocator) {
  if (!asset.generator.empty()) {
    SerializeStringProperty("generator", asset.generator, o, allocator);
  }

  if (!asset.version.empty()) {
    SerializeStringProperty("version", asset.version, o, allocator);
  }

  if (asset.extras.Keys().size()) {
    SerializeValue("extras", asset.extras, o, allocator);
  }

  SerializeExtensionMap(asset.extensions, o, allocator);
}

static void SerializeGltfBuffer(Buffer &buffer, json::Value &o,
                                json::Doc::AllocatorType &allocator) {
  SerializeNumberProperty("byteLength", buffer.data.size(), o, allocator);
  SerializeGltfBufferData(buffer.data, o, allocator);

  if (buffer.name.size())
    SerializeStringProperty("name", buffer.name, o, allocator);

  if (buffer.extras.Type() != NULL_TYPE) {
    SerializeValue("extras", buffer.extras, o, allocator);
  }
}

static bool SerializeGltfBuffer(Buffer &buffer, json::Value &o,
                                const ptl::string &binFilename,
                                const ptl::string &binBaseFilename,
                                json::Doc::AllocatorType &allocator) {
  if (!SerializeGltfBufferData(buffer.data, binFilename)) return false;
  SerializeNumberProperty("byteLength", buffer.data.size(), o, allocator);
  SerializeStringProperty("uri", binBaseFilename, o, allocator);

  if (buffer.name.size())
    SerializeStringProperty("name", buffer.name, o, allocator);

  if (buffer.extras.Type() != NULL_TYPE) {
    SerializeValue("extras", buffer.extras, o, allocator);
  }
  return true;
}

static void SerializeGltfBufferView(BufferView &bufferView, json::Value &o,
                                    json::Doc::AllocatorType &allocator) {
  SerializeNumberProperty("buffer", bufferView.buffer, o, allocator);
  SerializeNumberProperty<size_t>("byteLength", bufferView.byteLength, o,
                                  allocator);

  
  if (bufferView.byteStride >= 4) {
    SerializeNumberProperty<size_t>("byteStride", bufferView.byteStride, o,
                                    allocator);
  }
  
  if (bufferView.byteOffset > 0) {
    SerializeNumberProperty<size_t>("byteOffset", bufferView.byteOffset, o,
                                    allocator);
  }
  
  if (bufferView.target == TINYGLTF_TARGET_ARRAY_BUFFER ||
      bufferView.target == TINYGLTF_TARGET_ELEMENT_ARRAY_BUFFER) {
    SerializeNumberProperty("target", bufferView.target, o, allocator);
  }
  if (bufferView.name.size()) {
    SerializeStringProperty("name", bufferView.name, o, allocator);
  }

  if (bufferView.extras.Type() != NULL_TYPE) {
    SerializeValue("extras", bufferView.extras, o, allocator);
  }
}

static void SerializeGltfImage(Image &image, json::Value &o,
                               json::Doc::AllocatorType &allocator) {
  SerializeStringProperty("uri", image.uri, o, allocator);

  if (image.name.size()) {
    SerializeStringProperty("name", image.name, o, allocator);
  }

  if (image.extras.Type() != NULL_TYPE) {
    SerializeValue("extras", image.extras, o, allocator);
  }

  SerializeExtensionMap(image.extensions, o, allocator);
}

static void SerializeGltfMaterial(Material &material, json::Value &o,
                                  json::Doc::AllocatorType &allocator) {
  if (material.extras.Size())
    SerializeValue("extras", material.extras, o, allocator);
  SerializeExtensionMap(material.extensions, o, allocator);

  if (material.values.size()) {
    json::Value pbrMetallicRoughness;
    SerializeParameterMap(material.values, pbrMetallicRoughness, allocator);
    o["pbrMetallicRoughness"] = pbrMetallicRoughness;
  }

  SerializeParameterMap(material.additionalValues, o, allocator);

  if (material.name.size()) {
    SerializeStringProperty("name", material.name, o, allocator);
  }

  if (material.extras.Type() != NULL_TYPE) {
    SerializeValue("extras", material.extras, o, allocator);
  }
}

static void SerializeGltfMesh(Mesh &mesh, json::Value &o,
                              json::Doc::AllocatorType &allocator) {
  json::Value primitives{rapidjson::Type::kArrayType};
  for (unsigned int i = 0; i < mesh.primitives.size(); ++i) {
    json::Value primitive{rapidjson::Type::kObjectType};
    json::Value attributes{rapidjson::Type::kObjectType};
    Primitive gltfPrimitive = mesh.primitives[i];
    for (ptl::map<ptl::string, int>::iterator attrIt =
             gltfPrimitive.attributes.begin();
         attrIt != gltfPrimitive.attributes.end(); ++attrIt) {
      SerializeNumberProperty<int>(attrIt->first, attrIt->second, attributes,
                                   allocator);
    }

    primitive.AddMember("attributes", attributes, allocator);

    
    if (gltfPrimitive.indices > -1) {
      SerializeNumberProperty<int>("indices", gltfPrimitive.indices, primitive,
                                   allocator);
    }
    
    if (gltfPrimitive.material > -1) {
      SerializeNumberProperty<int>("material", gltfPrimitive.material,
                                   primitive, allocator);
    }
    SerializeNumberProperty<int>("mode", gltfPrimitive.mode, primitive,
                                 allocator);

    
    if (gltfPrimitive.targets.size()) {
      json::Value targets;
      for (unsigned int k = 0; k < gltfPrimitive.targets.size(); ++k) {
        json::Value targetAttributes;
        ptl::map<ptl::string, int> targetData = gltfPrimitive.targets[k];
        for (ptl::map<ptl::string, int>::iterator attrIt = targetData.begin();
             attrIt != targetData.end(); ++attrIt) {
          SerializeNumberProperty<int>(attrIt->first, attrIt->second,
                                       targetAttributes, allocator);
        }

        targets.PushBack(targetAttributes, allocator);
      }
      primitive["targets"] = targets;
    }

    if (gltfPrimitive.extras.Type() != NULL_TYPE) {
      SerializeValue("extras", gltfPrimitive.extras, primitive, allocator);
    }

    primitives.PushBack(primitive, allocator);
  }

  o.AddMember("primitives", primitives, allocator);
  if (mesh.weights.size()) {
    SerializeNumberArrayProperty<double>("weights", mesh.weights, o, allocator);
  }

  if (mesh.name.size()) {
    SerializeStringProperty("name", mesh.name, o, allocator);
  }

  if (mesh.extras.Type() != NULL_TYPE) {
    SerializeValue("extras", mesh.extras, o, allocator);
  }
}

static void SerializeGltfLight(Light &light, json::Value &o,
                               json::Doc::AllocatorType &allocator) {
  if (!light.name.empty())
    SerializeStringProperty("name", light.name, o, allocator);
  SerializeNumberArrayProperty("color", light.color, o, allocator);
  SerializeStringProperty("type", light.type, o, allocator);
}

static void SerializeGltfNode(Node &node, json::Value &o,
                              json::Doc::AllocatorType &allocator) {
  if (node.translation.size() > 0) {
    SerializeNumberArrayProperty<double>("translation", node.translation, o,
                                         allocator);
  }
  if (node.rotation.size() > 0) {
    SerializeNumberArrayProperty<double>("rotation", node.rotation, o,
                                         allocator);
  }
  if (node.scale.size() > 0) {
    SerializeNumberArrayProperty<double>("scale", node.scale, o, allocator);
  }
  if (node.matrix.size() > 0) {
    SerializeNumberArrayProperty<double>("matrix", node.matrix, o, allocator);
  }
  if (node.mesh != -1) {
    SerializeNumberProperty<int>("mesh", node.mesh, o, allocator);
  }

  if (node.skin != -1) {
    SerializeNumberProperty<int>("skin", node.skin, o, allocator);
  }

  if (node.camera != -1) {
    SerializeNumberProperty<int>("camera", node.camera, o, allocator);
  }

  if (node.extras.Type() != NULL_TYPE) {
    SerializeValue("extras", node.extras, o, allocator);
  }

  SerializeExtensionMap(node.extensions, o, allocator);
  if (!node.name.empty())
    SerializeStringProperty("name", node.name, o, allocator);
  SerializeNumberArrayProperty<int>("children", node.children, o, allocator);
}

static void SerializeGltfSampler(Sampler &sampler, json::Value &o,
                                 json::Doc::AllocatorType &allocator) {
  SerializeNumberProperty("magFilter", sampler.magFilter, o, allocator);
  SerializeNumberProperty("minFilter", sampler.minFilter, o, allocator);
  SerializeNumberProperty("wrapR", sampler.wrapR, o, allocator);
  SerializeNumberProperty("wrapS", sampler.wrapS, o, allocator);
  SerializeNumberProperty("wrapT", sampler.wrapT, o, allocator);

  if (sampler.extras.Type() != NULL_TYPE) {
    SerializeValue("extras", sampler.extras, o, allocator);
  }
}

static void SerializeGltfOrthographicCamera(
    const OrthographicCamera &camera, json::Value &o,
    json::Doc::AllocatorType &allocator) {
  SerializeNumberProperty("zfar", camera.zfar, o, allocator);
  SerializeNumberProperty("znear", camera.znear, o, allocator);
  SerializeNumberProperty("xmag", camera.xmag, o, allocator);
  SerializeNumberProperty("ymag", camera.ymag, o, allocator);

  if (camera.extras.Type() != NULL_TYPE) {
    SerializeValue("extras", camera.extras, o, allocator);
  }
}

static void SerializeGltfPerspectiveCamera(
    const PerspectiveCamera &camera, json::Value &o,
    json::Doc::AllocatorType &allocator) {
  SerializeNumberProperty("zfar", camera.zfar, o, allocator);
  SerializeNumberProperty("znear", camera.znear, o, allocator);
  if (camera.aspectRatio > 0) {
    SerializeNumberProperty("aspectRatio", camera.aspectRatio, o, allocator);
  }

  if (camera.yfov > 0) {
    SerializeNumberProperty("yfov", camera.yfov, o, allocator);
  }

  if (camera.extras.Type() != NULL_TYPE) {
    SerializeValue("extras", camera.extras, o, allocator);
  }
}

static void SerializeGltfCamera(const Camera &camera, json::Value &o,
                                json::Doc::AllocatorType &allocator) {
  SerializeStringProperty("type", camera.type, o, allocator);
  if (!camera.name.empty()) {
    SerializeStringProperty("name", camera.name, o, allocator);
  }

  if (camera.type.compare("orthographic") == 0) {
    json::Value orthographic;
    SerializeGltfOrthographicCamera(camera.orthographic, orthographic,
                                    allocator);
    o["orthographic"] = orthographic;
  } else if (camera.type.compare("perspective") == 0) {
    json::Value perspective;
    SerializeGltfPerspectiveCamera(camera.perspective, perspective, allocator);
    o["perspective"] = perspective;
  } else {
    
  }
}

static void SerializeGltfScene(Scene &scene, json::Value &o,
                               json::Doc::AllocatorType &allocator) {
  SerializeNumberArrayProperty<int>("nodes", scene.nodes, o, allocator);

  if (scene.name.size()) {
    SerializeStringProperty("name", scene.name, o, allocator);
  }
  if (scene.extras.Type() != NULL_TYPE) {
    SerializeValue("extras", scene.extras, o, allocator);
  }
  SerializeExtensionMap(scene.extensions, o, allocator);
}

static void SerializeGltfSkin(Skin &skin, json::Value &o,
                              json::Doc::AllocatorType &allocator) {
  if (skin.inverseBindMatrices != -1)
    SerializeNumberProperty("inverseBindMatrices", skin.inverseBindMatrices, o,
                            allocator);

  SerializeNumberArrayProperty<int>("joints", skin.joints, o, allocator);
  SerializeNumberProperty("skeleton", skin.skeleton, o, allocator);
  if (skin.name.size()) {
    SerializeStringProperty("name", skin.name, o, allocator);
  }
}

static void SerializeGltfTexture(Texture &texture, json::Value &o,
                                 json::Doc::AllocatorType &allocator) {
  if (texture.sampler > -1) {
    SerializeNumberProperty("sampler", texture.sampler, o, allocator);
  }
  if (texture.source > -1) {
    SerializeNumberProperty("source", texture.source, o, allocator);
  }
  if (texture.extras.Type() != NULL_TYPE) {
    SerializeValue("extras", texture.extras, o, allocator);
  }
  SerializeExtensionMap(texture.extensions, o, allocator);
}

static bool WriteGltfFile(const ptl::string &output,
                          const ptl::string &content) {
  std::ofstream gltfFile(output.c_str());
  if (!gltfFile.is_open()) return false;
  gltfFile << content << std::endl;
  return true;
}

static void WriteBinaryGltfFile(const ptl::string &output,
                                const ptl::string &content) {
  std::ofstream gltfFile(output.c_str(), std::ios::binary);

  const ptl::string header = "glTF";
  const int version = 2;
  const int padding_size = content.size() % 4;

  
  
  const int length = 12 + 8 + int(content.size()) + padding_size;

  gltfFile.write(header.c_str(), header.size());
  gltfFile.write(reinterpret_cast<const char *>(&version), sizeof(version));
  gltfFile.write(reinterpret_cast<const char *>(&length), sizeof(length));

  
  const int model_length = int(content.size()) + padding_size;
  const int model_format = 0x4E4F534A;
  gltfFile.write(reinterpret_cast<const char *>(&model_length),
                 sizeof(model_length));
  gltfFile.write(reinterpret_cast<const char *>(&model_format),
                 sizeof(model_format));
  gltfFile.write(content.c_str(), content.size());

  
  if (padding_size > 0) {
    const ptl::string padding = ptl::string(padding_size, ' ');
    gltfFile.write(padding.c_str(), padding.size());
  }
}

bool TinyGLTF::WriteGltfSceneToFile(Model *model, const ptl::string &filename,
                                    bool embedImages = false,
                                    bool embedBuffers = false,
                                    bool prettyPrint = true,
                                    bool writeBinary = false) {
  json::Value output{rapidjson::Type::kObjectType};
  json::Doc document;

  RAPIDJSON_ASSERT(
      false && "This is currently bugged! the material name is duplicated!!!");

  
  json::Value accessors{rapidjson::Type::kArrayType};
  for (unsigned int i = 0; i < model->accessors.size(); ++i) {
    json::Value accessor{rapidjson::Type::kObjectType};
    SerializeGltfAccessor(model->accessors[i], accessor,
                          document.GetAllocator());
    accessors.PushBack(accessor, document.GetAllocator());
  }
  output.AddMember("accessors", accessors, document.GetAllocator());

  
  if (model->animations.size()) {
    json::Value animations;
    for (unsigned int i = 0; i < model->animations.size(); ++i) {
      if (model->animations[i].channels.size()) {
        json::Value animation;
        SerializeGltfAnimation(model->animations[i], animation,
                               document.GetAllocator());
        animations.PushBack(animation, document.GetAllocator());
      }
    }
    output["animations"] = animations;
  }

  
  json::Value asset{rapidjson::Type::kObjectType};
  SerializeGltfAsset(model->asset, asset, document.GetAllocator());
  output.AddMember("asset", asset, document.GetAllocator());

  ptl::string defaultBinFilename = GetBaseFilename(filename);
  ptl::string defaultBinFileExt = ".bin";
  ptl::string::size_type pos =
      defaultBinFilename.rfind('.', defaultBinFilename.length());

  if (pos != ptl::string::npos) {
    defaultBinFilename = defaultBinFilename.substr(0, pos);
  }
  ptl::string baseDir = GetBaseDir(filename);
  if (baseDir.empty()) {
    baseDir = "./";
  }

  
  ptl::vector<ptl::string> usedUris;
  json::Value buffers{rapidjson::Type::kArrayType};
  for (unsigned int i = 0; i < model->buffers.size(); ++i) {
    json::Value buffer{rapidjson::Type::kObjectType};
    if (embedBuffers) {
      SerializeGltfBuffer(model->buffers[i], buffer, document.GetAllocator());
    } else {
      ptl::string binSavePath;
      ptl::string binUri;
      if (!model->buffers[i].uri.empty() && !IsDataURI(model->buffers[i].uri)) {
        binUri = model->buffers[i].uri;
      } else {
        binUri = defaultBinFilename + defaultBinFileExt;
        bool inUse = true;
        int numUsed = 0;
        while (inUse) {
          inUse = false;
          for (const ptl::string &usedName : usedUris) {
            if (binUri.compare(usedName) != 0) continue;
            inUse = true;
            binUri = defaultBinFilename + ptl::to_string(numUsed++) +
                     defaultBinFileExt;
            break;
          }
        }
      }
      usedUris.push_back(binUri);
      binSavePath = JoinPath(baseDir, binUri);
      if (!SerializeGltfBuffer(model->buffers[i], buffer, binSavePath, binUri,
                               document.GetAllocator())) {
        return false;
      }
    }
    buffers.PushBack(buffer, document.GetAllocator());
  }
  output.AddMember("buffers", buffers, document.GetAllocator());

  
  json::Value bufferViews{rapidjson::Type::kArrayType};
  for (unsigned int i = 0; i < model->bufferViews.size(); ++i) {
    json::Value bufferView{rapidjson::Type::kObjectType};
    SerializeGltfBufferView(model->bufferViews[i], bufferView,
                            document.GetAllocator());
    bufferViews.PushBack(bufferView, document.GetAllocator());
  }
  output.AddMember("bufferViews", bufferViews, document.GetAllocator());

  
  if (model->extensionsUsed.size()) {
    SerializeStringArrayProperty("extensionsUsed", model->extensionsUsed,
                                 output, document.GetAllocator());
  }

  
  if (model->extensionsRequired.size()) {
    SerializeStringArrayProperty("extensionsRequired",
                                 model->extensionsRequired, output,
                                 document.GetAllocator());
  }

  
  if (model->images.size()) {
    json::Value images{rapidjson::Type::kArrayType};
    for (unsigned int i = 0; i < model->images.size(); ++i) {
      json::Value image{rapidjson::Type::kObjectType};

      UpdateImageObject(model->images[i], baseDir, int(i), embedImages,
                        &this->WriteImageData, this->write_image_user_data_);
      SerializeGltfImage(model->images[i], image, document.GetAllocator());
      images.PushBack(image, document.GetAllocator());
    }
    output.AddMember("images", images, document.GetAllocator());
  }

  
  if (model->materials.size()) {
    json::Value materials{rapidjson::Type::kArrayType};
    for (unsigned int i = 0; i < model->materials.size(); ++i) {
      json::Value material{rapidjson::Type::kObjectType};
      SerializeGltfMaterial(model->materials[i], material,
                            document.GetAllocator());
      materials.PushBack(material, document.GetAllocator());
    }
    output.AddMember("materials", materials, document.GetAllocator());
  }

  
  if (model->meshes.size()) {
    json::Value meshes{rapidjson::Type::kArrayType};
    for (unsigned int i = 0; i < model->meshes.size(); ++i) {
      json::Value mesh{rapidjson::Type::kObjectType};
      SerializeGltfMesh(model->meshes[i], mesh, document.GetAllocator());
      meshes.PushBack(mesh, document.GetAllocator());
    }
    output.AddMember("meshes", meshes, document.GetAllocator());
  }

  
  if (model->nodes.size()) {
    json::Value nodes{rapidjson::Type::kArrayType};
    for (unsigned int i = 0; i < model->nodes.size(); ++i) {
      json::Value node{rapidjson::Type::kObjectType};
      SerializeGltfNode(model->nodes[i], node, document.GetAllocator());
      nodes.PushBack(node, document.GetAllocator());
    }
    output.AddMember("nodes", nodes, document.GetAllocator());
  }

  
  if (model->defaultScene > -1) {
    SerializeNumberProperty<int>("scene", model->defaultScene, output,
                                 document.GetAllocator());
  }

  
  if (model->scenes.size()) {
    json::Value scenes{rapidjson::Type::kArrayType};
    for (unsigned int i = 0; i < model->scenes.size(); ++i) {
      json::Value currentScene{rapidjson::Type::kObjectType};
      SerializeGltfScene(model->scenes[i], currentScene,
                         document.GetAllocator());
      scenes.PushBack(currentScene, document.GetAllocator());
    }
    output.AddMember("scenes", scenes, document.GetAllocator());
  }

  
  if (model->skins.size()) {
    json::Value skins{rapidjson::Type::kArrayType};
    for (unsigned int i = 0; i < model->skins.size(); ++i) {
      json::Value skin{rapidjson::Type::kObjectType};
      SerializeGltfSkin(model->skins[i], skin, document.GetAllocator());
      skins.PushBack(skin, document.GetAllocator());
    }
    output.AddMember("skins", skins, document.GetAllocator());
  }

  
  if (model->textures.size()) {
    json::Value textures{rapidjson::Type::kArrayType};
    for (unsigned int i = 0; i < model->textures.size(); ++i) {
      json::Value texture{rapidjson::Type::kObjectType};
      SerializeGltfTexture(model->textures[i], texture,
                           document.GetAllocator());
      textures.PushBack(texture, document.GetAllocator());
    }
    output.AddMember("textures", textures, document.GetAllocator());
  }

  
  if (model->samplers.size()) {
    json::Value samplers{rapidjson::Type::kArrayType};
    for (unsigned int i = 0; i < model->samplers.size(); ++i) {
      json::Value sampler{rapidjson::Type::kObjectType};
      SerializeGltfSampler(model->samplers[i], sampler,
                           document.GetAllocator());
      samplers.PushBack(sampler, document.GetAllocator());
    }
    output.AddMember("samplers", samplers, document.GetAllocator());
  }

  
  if (model->cameras.size()) {
    json::Value cameras{rapidjson::Type::kArrayType};
    for (unsigned int i = 0; i < model->cameras.size(); ++i) {
      json::Value camera{rapidjson::Type::kObjectType};
      SerializeGltfCamera(model->cameras[i], camera, document.GetAllocator());
      cameras.PushBack(camera, document.GetAllocator());
    }
    output.AddMember("cameras", cameras, document.GetAllocator());
  }

  
  SerializeExtensionMap(model->extensions, output, document.GetAllocator());

  
  if (model->lights.size()) {
    json::Value lights{rapidjson::Type::kArrayType};
    for (unsigned int i = 0; i < model->lights.size(); ++i) {
      json::Value light{rapidjson::Type::kObjectType};
      SerializeGltfLight(model->lights[i], light, document.GetAllocator());
      lights.PushBack(light, document.GetAllocator());
    }
    json::Value khr_lights_cmn{rapidjson::Type::kObjectType};
    khr_lights_cmn.AddMember("lights", lights, document.GetAllocator());
    json::Value ext_j;

    if (output.FindMember("extensions") != output.MemberEnd()) {
      ext_j.CopyFrom(output["extensions"], document.GetAllocator());
    }

    ext_j.AddMember("KHR_lights_cmn", khr_lights_cmn, document.GetAllocator());

    if (output.FindMember("extensions") != output.MemberEnd())
      output["extensions"].CopyFrom(ext_j, document.GetAllocator());
    else
      output.AddMember("extensions", ext_j, document.GetAllocator());
  }

  
  if (model->extras.Type() != NULL_TYPE) {
    SerializeValue("extras", model->extras, output, document.GetAllocator());
  }

  rapidjson::StringBuffer buffer;
  rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
  output.Accept(writer);

  if (writeBinary) {
    WriteBinaryGltfFile(filename, buffer.GetString());
  } else {
    WriteGltfFile(filename,
                  buffer.GetString() );
  }

  return true;
}

}  

#ifdef __clang__
#pragma clang diagnostic pop
#endif



#endif  

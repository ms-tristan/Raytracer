#ifndef SRC_SCENE_SCENEDIRECTOR_CONFIGPARSER_HPP_
#define SRC_SCENE_SCENEDIRECTOR_CONFIGPARSER_HPP_

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <libconfig.h++>
#include "Math/Vector3D/Vector3D.hpp"
#include "Math/Point3D/Point3D.hpp"
#include "Scene/SceneBuilder/SceneBuilder.hpp"

namespace RayTracer {

class ConfigParser {
 public:
   virtual ~ConfigParser() = default;
   virtual void parse(const libconfig::Setting& setting, SceneBuilder& builder) = 0;

 protected:
   template<typename T>
   T getValueOrDefault(const libconfig::Setting& setting,
                        const std::string& key,
                        const T& defaultValue) {
      if (!setting.exists(key))
         return defaultValue;
      return static_cast<T>(setting[key.c_str()]);
   }

   Math::Point3D parsePoint3D(const libconfig::Setting& setting);
   Math::Vector3D parseVector3D(const libconfig::Setting& setting);
   Math::Vector3D parseColor(const libconfig::Setting& setting);
};

class CameraParser : public ConfigParser {
 public:
   void parse(const libconfig::Setting& setting, SceneBuilder& builder) override;
};

class LightsParser : public ConfigParser {
 public:
   void parse(const libconfig::Setting& setting, SceneBuilder& builder) override;

 private:
   void parsePointLights(const libconfig::Setting& lights, SceneBuilder& builder);
   void parseDirectionalLights(const libconfig::Setting& lights, SceneBuilder& builder);
};

class PrimitivesParser : public ConfigParser {
 public:
   void parse(const libconfig::Setting& setting, SceneBuilder& builder) override;

 private:
   void parsePluginPrimitives(const std::string& typeName, const libconfig::Setting& primitives,
                              SceneBuilder& builder);
   std::map<std::string, double> extractParametersFromSetting(
         const libconfig::Setting& setting,
         const std::vector<std::string>& requiredParams);
   std::shared_ptr<Material> extractMaterialFromSetting(const libconfig::Setting& setting);
};

class ShadersParser : public ConfigParser {
 public:
   void parse(const libconfig::Setting& setting, SceneBuilder& builder) override;

 private:
   void parsePluginShader(const libconfig::Setting& shader, SceneBuilder& builder);
};

class PostProcessParser : public ConfigParser {
 public:
   void parse(const libconfig::Setting& setting, SceneBuilder& builder) override;

 private:
   void parsePluginPostProcess(const libconfig::Setting& postProcess, SceneBuilder& builder);
   std::map<std::string, double> extractParametersFromSetting(
         const libconfig::Setting& setting,
         const std::vector<std::string>& requiredParams);
};

class SceneConfigParser {
 public:
   SceneConfigParser();
   std::unique_ptr<Scene> parseFile(const std::string& filename);

 private:
   SceneBuilder builder;
   std::unordered_map<std::string, std::unique_ptr<ConfigParser>> sectionParsers;
};

}  // namespace RayTracer

#endif  // SRC_SCENE_SCENEDIRECTOR_CONFIGPARSER_HPP_
# Hoedown构建
aux_source_directory(${HOEDOWN_DIR}/src HOEDOWN_SOURCES)
if(MSVC)
  add_library(hoedown STATIC ${HOEDOWN_SOURCES})
else()
  add_library(hoedown SHARED ${HOEDOWN_SOURCES})
endif()

# Yaml构建
aux_source_directory(${YAML_DIR}/src YAML_SOURCES)
aux_source_directory(${YAML_DIR}/include/yaml-cpp YAML_HEADERS)
aux_source_directory(${YAML_DIR}/include/yaml-cpp/contrib YAML_PRIVATE_HEADERS)
aux_source_directory(${YAML_DIR}/src/contrib YAML_PRIVATE_SOURCES)
if(MSVC)
  add_library(yaml STATIC ${YAML_HEADERS} ${YAML_PRIVATE_HEADERS}
    ${YAML_SOURCES} ${YAML_PRIVATE_SOURCES})
else()
  add_library(yaml SHARED ${YAML_HEADERS} ${YAML_PRIVATE_HEADERS}
    ${YAML_SOURCES} ${YAML_PRIVATE_SOURCES})
endif()
find_path(NODEJS_INCLUDE_DIR  node.h
  PATHS
    /usr/include/nodejs/src
  )
find_path(NODEJS_UV_INCLUDE_DIR  uv.h
  PATHS
    /usr/include/nodejs/deps/uv/include/
  )

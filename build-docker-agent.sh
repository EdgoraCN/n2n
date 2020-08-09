docker build -t registry.cn-beijing.aliyuncs.com/edgora-oss/edge --no-cache -f Dockerfile.agent .

docker tag  registry.cn-beijing.aliyuncs.com/edgora-oss/edge  registry.cn-beijing.aliyuncs.com/edgora-oss/edge:2.6.1

docker push  registry.cn-beijing.aliyuncs.com/edgora-oss/edge

docker push  registry.cn-beijing.aliyuncs.com/edgora-oss/edge:2.6.1


docker tag  registry.cn-beijing.aliyuncs.com/edgora-oss/edge:2.6.1  edgora/edge:2.6.1
docker push edgora/edge:2.6.1

docker build -t registry.cn-beijing.aliyuncs.com/edgora-oss/n2n -f Dockerfile .

docker tag  registry.cn-beijing.aliyuncs.com/edgora-oss/n2n  registry.cn-beijing.aliyuncs.com/edgora-oss/n2n:2.6

docker push  registry.cn-beijing.aliyuncs.com/edgora-oss/n2n

docker push  registry.cn-beijing.aliyuncs.com/edgora-oss/n2n:2.6
#!/bin/sh

POSITIONAL=()
while [[ $# -gt 0 ]]
do
key="$1"

case $key in
    -u|--username)
    edge_username="$2"
    shift # past argument
    shift # past value
    ;;
    -i|--id)
    edge_id="$2"
    shift # past argument
    shift # past value
    ;;
    -p|--code)
    edge_code="$2"
    shift # past argument
    shift # past value
    ;;
    --default)
    DEFAULT=YES
    shift # past argument
    ;;
    *)    # unknown option
    POSITIONAL+=("$1") # save it in an array for later
    shift # past argument
    ;;
esac
done
set -- "${POSITIONAL[@]}" # restore positional parameters

echo edge_username=$edge_username
echo edge_id=$edge_id
export edge_host="https://console.edgora.com"
#export edge_host="http://10.9.9.120:9000"

# shutdown old version
if [ -x "$(command -v systemctl)" ]; then
    sudo systemctl stop edge-agent.service
    sudo systemctl disable edge-agent.service
fi

getPid(){
        pid=$(ps -ef | grep "$1" | grep -v grep | awk '{print $2}')
        echo "$pid"
}
pid=`getPid "edge-agent-linux"`
echo $pid
if [ ! -z "$pid" ]; then
        kill -9  $pid
        echo "edge-agent  is killed"
else
    echo "edge-agent is not running"
fi

# download new version
mkdir -p   ~/.edge-agent
curl -o ~/.edge-agent/edge-agent.linux.tar.gz "http://file.edgora.cn/edge-agent.linux.tar.gz?buildId=2.6.1"
tar -C ~/.edge-agent  -xvf ~/.edge-agent/edge-agent.linux.tar.gz
chmod +x ~/.edge-agent/{edge-agent-linux,libcrypt.so}
# dowanlod jj
curl -o ~/.edge-agent/jj-1.2.3-linux-amd64.tar.gz "http://file.edgora.cn/jj-1.2.3-linux-amd64.tar.gz"
tar -C ~/.edge-agent  -xvf ~/.edge-agent/jj-1.2.3-linux-amd64.tar.gz
mv ~/.edge-agent/jj-1.2.3-linux-amd64/jj ~/.edge-agent/jj
chmod +x ~/.edge-agent/jj
export PATH=$HOME/.edge-agent:$PATH

echo "try to active edge"
curl -v -o ~/.edge-agent/edge-agent.json $edge_host/api/client-tokens/active/$edge_username/$edge_id/$edge_code

token=`cat ~/.edge-agent/edge-agent.json | ~/.edge-agent/jj token`
if [ -z "$token" ]; then
    echo "token is not found, exit"
    cat  ~/.edge-agent/edge-agent.json
    exit 1;
fi

curl -v -o ~/.edge-agent/edge-agent.conf $edge_host/api/edge/conf/$token


# startup service
if [ -x "$(command -v systemctl)" ]; then
    cd ~/.edge-agent
    sed -i "s|%h|$HOME|g" scripts/edge-agent.service
    sed -i "s|%u|$USER|g" scripts/edge-agent.service
    sudo cp -fr  ~/.edge-agent/scripts/edge-agent.service  /etc/systemd/system/edge-agent.service
    sudo systemctl  enable edge-agent.service
    sudo systemctl  restart edge-agent.service
    sleep 3
    sudo systemctl status  edge-agent.service
else 
    cd ~/.edge-agent &&  ./edge-agent-linux $HOME/.edge-agent/edge-agent.conf
fi



#!/bin/bash

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
mkdir -p /tmp

FILE=/tmp/edge-agent.conf
if  [ -f "$FILE" ]; then
    echo "edge config file [$FILE] exists, starting..."
    edge $FILE
else 
    echo "try to active edge"
    curl -o /tmp/edge-agent.json https://console.edgora.com/api/client-tokens/active/$edge_username/$edge_id/$edge_code
    token=`cat /tmp/edge-agent.json | jj token`
    if [ -z "$token" ]; then
        echo "token is not found, exit"
        cat  ~/.edge-agent/edge-agent.json
        exit 1;
    fi
    curl -o ~/.edge-agent/edge-agent.conf http://10.9.9.120:9000/api/edge/conf/$token
    edge $FILE
fi

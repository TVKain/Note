#!/bin/bash

# Tạo hai namespace *dev* và *prod*
ip netns add dev
ip netns add prod

# Tạo và bật bridge *v-net-0*
ip link add v-net-0 type bridge 
ip link set dev v-net-0 up

# Tạo cặp giao diện để liên kết từ *dev* tới bridge
ip link add veth-dev type veth peer name veth-dev-br

# Tạo cặp giao diện để liên kết từ *prod* tới bridge
ip link add veth-prod type veth peer name veth-prod-br

# Chuyển giao diện *veth-dev* sang namespace *dev*
ip link set veth-dev netns dev

# Gắn giao diện *veth-dev-br* vào bridge *v-net-0*
ip link set veth-dev-br master v-net-0

# Chuyển giao diện *veth-prod* sang namespace *prod*
ip link set veth-prod netns prod

# Gắn giao diện *veth-prod-br* vào bridge *v-net-0*
ip link set veth-prod-br master v-net-0

# Gán địa chỉ ip cho mỗi giao diện của namespace *dev* và *prod*
ip -n dev addr add 192.168.10.1/24 dev veth-dev
ip -n prod addr add 192.168.10.2/24 dev veth-prod

# Bật các giao diện 
ip -n dev link set veth-dev up
ip -n prod link set veth-prod up
ip link set veth-dev-br up
ip link set veth-prod-br up

# Thử ping từ namespace dev tới namespace prod
ip netns exec dev ping 192.168.10.2
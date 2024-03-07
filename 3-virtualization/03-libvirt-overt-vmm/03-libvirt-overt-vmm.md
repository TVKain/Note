# Cài đặt KVM Hypervisor, libvirt và oVirt 

## 1. Làm quen QEMU và libvirt 

- Sử dụng QEMU làm virtualizer (thực thi code máy ảo trực tiếp trên CPU vật lý -> hiệu năng gần như là native, ít overhead)
- Sử dụng libvirt làm nền tảng quản lý trên QEMU, với câu lệnh *virsh* chúng ta có thể quản trị máy ảo, mạng ảo,... 

## 2. Làm quen oVirt 

- oVirt là nền tảng mã nguồn mở quản lý môi trường KVM 
- Công cụ GUI giúp quản lý môi trường KVM từ một console web tập trung 
- oVirt có hai thành phần chính 
    - Engine: kết nối tới thông qua giao diện GUI 
    - Agents: oVirt sử dụng các agents để giao tiếp với các hosts 


## 3. Cài đặt QEMU, libvirt, oVirt 
TODO

---
# install libselinux-python on centos7
- name: Install net-tools
  yum: name=net-tools
  when:
    - "ansible_os_family == 'RedHat'"

# Set vm.swappiness to 1 in /etc/sysctl.conf
- sysctl:
    name: vm.swappiness
    value: "{{ swapness }}"
    state: present

# Set virtual max area /etc/sysctl.conf
- sysctl:
    name: vm.max_map_count
    value: "{{ vm_max_count }}"
    state: present

# Download elasticsearch
- name: Download rpm.
  get_url:
    url: https://artifacts.elastic.co/downloads/{{ esurl }}.rpm
    dest: /tmp/elasticsearch-{{ elasticsearch_version }}.rpm

# enable openfile limit without restart
- name: enable open file witout restart
  shell: "ulimit -n 4096"

# Install rpm file elasticsearch
- name: install elasticsearch
  yum: name=/tmp/elasticsearch-{{ elasticsearch_version }}.rpm state=present
  notify:
    - restart elasticsearch

# Create directory for es_data and heap_dump storage
- name: Create data_path directory
  file:
    path: "{{ item }}"
    state: directory
    owner: elasticsearch
    group: elasticsearch
    mode: 0775
    recurse: yes
  with_items:
    - "{{ data_path }}"
    - "{{ es_jvm_dump }}"
  notify:
    - restart elasticsearch

# Enable elaticsearch
- name: Enable elasticsearch
  systemd: name=elasticsearch enabled=yes

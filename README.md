# osm_elasticSearch_Cluster
This repo controls the Ansible Role for ElasticSearch cluster installation.

# Supported OS  

```
Redhat 7
Centos 7
Ubuntu 14/16
Amazon Linux
```

# Dependencies
pyhton  
Java 1.8 or greater 

# Variables/Default for elasticsearch
change below information and uncomment it based on your requirement in var/main.yml file:

```
elasticsearch_version: "6.5.1"
cluster_name: es-cluster

#es_jvm_dump: /var/lib/elasticsearch/heap
#data_path: /var/lib/elasticsearch
#log_path: /var/log/elasticsearch

#elasticsearchIP: "0.0.0.0"
```

# Node defination:
Define your type of nodes in host/inventory file based on below format. 

```
[es:children]
esm
esd
esc
[esm]
64.627.16.131 ansible_ssh_user=ubuntu
103.23.36.76 ansible_ssh_user=ubuntu
61.211.23.11 ansible_ssh_user=ubuntu
[esd]
54.147.131.37 ansible_ssh_user=ubuntu
45.62.49.102 ansible_ssh_user=ubuntu
```
Client node configuration is Optional, if required add below info in host/inventory file:
```
[esc]
100.26.208.53 ansible_ssh_user=ubuntu
```
Where,
es: is cluster group name
esm: is master node information
esd: is data node information
esc: is client node information

# Example Playbook

```
- hosts: es
  roles:
     - { role: osm_elasticsearch_cluster }
```

# License

BSD

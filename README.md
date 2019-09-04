# osm_elasticSearch_Cluster
This repo controls the Ansible Role for ElasticSearch cluster installation.

# Supported OS  

```
Redhat 7
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

#esIP: "0.0.0.0"
```

# Node defination:
Define your type of nodes in host/inventory file based on below format. Can increase the node counts as much you want. 

```
[es]
127.0.0.1 ansible_ssh_user=root
127.0.0.2 ansible_ssh_user=root
127.0.0.3 ansible_ssh_user=root
[esm]
127.0.0.1 ansible_ssh_user=root
127.0.0.2 ansible_ssh_user=root
[esd]
127.0.0.1 ansible_ssh_user=root
127.0.0.2 ansible_ssh_user=root
[esc]
127.0.0.3 ansible_ssh_user=root

```
Where,  
es: all es servers
esm: is master node information  
esd: is data node information  
esc: is client node information



add all es servers ip under group es. The add the specific server ip whoom u want to be master node in the group [esm], special mention this server need to be present in [es] group.do the same for data nodd and client node.

Remember that one same server can be  used for data and master node but for client we need a specific server.  

# Example Playbook

```
---
- hosts: all
  become: true
  roles:
    - osm_elasticsearch_cluster

```

# License

BSD

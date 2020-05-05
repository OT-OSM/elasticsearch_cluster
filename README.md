## Osm_ElasticSearch_Cluster

This repo controls the Ansible Role for ElasticSearch cluster installation.

## Key Features

Standalone setup
Cluster setup
TLS encryption

## Supported os

Redhat 7
Ubuntu 14/16/18
Amazon Linux

## Dependencies

python   
Java 1.8 or greater

## Role Variables

We have categorized variables into two part i.e. Manadatory and Optional

## Mandatory Variables

| **Variable** | **Default Value** | **Possible Values** | **Description** |
|--------------| ----------------- | ------------------- | --------------- |
| elasticsearch_version|7.3.1|Any es version as required|Exact es version which we need to install|
|elasticsearch_series|7|6/7/8 etc|Es major version|
|cluster_name|es-cluster|Cluster name |Cluster name |
|http_port|9200|Any Linux port|Assign a port to connect with es|
|tcp_port|9300|Any Linux port|Port through es node will communicate between them|
|tls_enable|no|Yes or no|This variable will enable or disable tls encryption |
|key_name|node1.key|Key name|Key name if tls encryption is enabled |
|certificate_name|node1.crt|Certificate name|Certificate name if tls encryption is enabled |
|certificate_authorities_name|ca.crt|Certificate authorities name|Certificate authorities name if tls encryption is enabled|
|nofile_limit|65536|65536|This limits the number of file descriptors any process owned by the specified domain can have open at any one time|
|nproc_limit|5000|5000|Nproc is defined at OS level to limit the number of processes per user|
|swapness|1|0 to 100|Swappiness is the kernel parameter that defines how much (and how often) your Linux kernel will copy RAM contents to swap. This parameter's default value is “60” and it can take anything from “0” to “100”. The higher the value of the swappiness parameter, the more aggressively your kernel will swap.|
|vm_max_count|2621444|2621444|Elasticsearch uses a mmapfs directory by default to store its indices. The default operating system limits on mmap counts is likely to be too low, which may result in out of memory exceptions.|
|m_lock|false|False or true|When the JVM does a major garbage collection it touches every page of the heap. If any of those pages are swapped out to disk they will have to be swapped back in to memory. That causes lots of disk thrashing that Elasticsearch would much rather use to service requests. There are several ways to configure a system to disallow swapping. One way is by requesting the JVM to lock the heap in memory through mlockall (Unix) or virtual lock (Windows). This is done via the Elasticsearch setting bootstrap.memory_lock|
|standalone|no|Yes or no|This variable is to mention standalone setup |


## Optional Variables

| **Variable** | **Default Value** | **Possible Values** | **Description** |
|--------------| ----------------- | ------------------- | --------------- |
|es_url|"elasticsearch/elasticsearch-{{ elasticsearch_version }}"|Url for es|Url for es|
|es_service|elasticsearch.service|Service file name|Service file name|
|es|elasticsearch|elasticsearch|Es name|
|es_jvm_dump|/var/lib/elasticsearch/heap|Any path|Path to jvm dump|
|log_path|/var/log/elasticsearch|Any path|Es log path|
|data_path|/var/lib/elasticsearch|Any path|Path to es data|
|systemd_path|/usr/lib/systemd/system|/usr/lib/systemd/system|service file path|
|conf|/etc/elasticsearch/|/etc/elasticsearch/|es config file path|


## Node defination:

Define your type of nodes in host/inventory file based on the below format. Can increase the node counts as much you want.

```
[es]
es1 ansible_host=127.0.0.1 ansible_ssh_user=root
es2 ansible_host=127.0.0.2 ansible_ssh_user=root
es3 ansible_host=127.0.0.3 ansible_ssh_user=root
[esm]
esm1 ansible_host=127.0.0.1 ansible_ssh_user=root
esm2 ansible_host=127.0.0.2 ansible_ssh_user=root
[esd]
esd1 ansible_host=127.0.0.1 ansible_ssh_user=root
esd2 ansible_host=127.0.0.2 ansible_ssh_user=root
[esc]
esc1 ansible_host=127.0.0.3 ansible_ssh_user=root

```
Where,  
es: all es servers
esm: is master node information  
esd: is data node information  
esc: is client node information



add all es servers IP under group es. The add the specific server ip whom u want to be a master node in the group [esm], special mention this server need to be present in [es] group.do the same for data node and client node.

Remember that one same server can be used for data and master node but for clients we need a specific server.  

## Example Playbook
```
---
- hosts: all
  become: true
  roles:
    - osm_elasticsearch_cluster
```

## Usage

There are multiple ways of executing the playbook according to your environment

To run the complete role
```

ansible-playbook -i hosts site.yml
```
To run with TLS encryption and vault
```
ansible-playbook -i host --ask-vault-pass site.yml
```

## TLS encryption enable
To enable tls encryption first got to the vars/main.yml file and change tls_enable variable value to “yes” then keep all certs in files directory and enter their name in vars/main.yml file. After that encrypt the certs with the vault. Run the bellow command and encrypt the certs and set a password.
```
ansible-vault encrypt ca.crt
```
Then run the role with the bellow command and u have provide the password that u have used to encrypt the certs.
```
ansible-playbook -i host --ask-vault-pass site.yml
```

you can use this password to encrypt and decrypt the certs
```
OcCeybCiWist33367
```

## Author

Moulendu Ghosh

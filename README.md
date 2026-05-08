# Elasticsearch Cluster Ansible Role (9.x)

This Ansible role automates the deployment of a secure, production-ready Elasticsearch 9.x cluster. It is designed to handle multi-node architectures with built-in support for TLS encryption, automated user management, and OS-level performance tuning.

## Key Features

- **Modern Standards**: Fully compliant with **FQCN** (Fully Qualified Collection Names) for long-term maintainability.
- **Secure by Default**: 
  - Automated **Transport and HTTP TLS/SSL** configuration.
  - Support for **externally provided certificates** (organization-issued) via the `push_certs` flag.
  - **Automated User Bootstrapping**: Automatically creates a custom administrative user with superuser privileges.
- **Robust Discovery**: Intelligent bootstrapping logic for `discovery.seed_hosts` and `cluster.initial_master_nodes` that handles both new clusters and single-node setups.
- **Performance Optimized**: Automatically configures critical OS settings (`vm.max_map_count`, `swappiness`, PAM limits) and manages JVM heap allocation.
- **Multi-OS Support**: Optimized for Ubuntu 20.04/22.04, Debian 11/12, and RHEL 8/9.

## Supported Operating Systems

- **Ubuntu**: 20.04 (Focal), 22.04 (Jammy)
- **Debian**: 11 (Bullseye), 12 (Bookworm)
- **RHEL/CentOS/Rocky**: 8, 9

## Prerequisites

- **Ansible Version**: 2.14 or higher recommended.
- **Collections**:
  - `ansible.posix`
  - `community.general`

## Role Variables

Variables are defined in `defaults/main.yml`. You should override these in `group_vars/all.yml` or your playbook.

### Core Configuration

| Variable | Default | Description |
|----------|---------|-------------|
| `elasticsearch_version` | `"9.3.4"` | The exact version of Elasticsearch to install. |
| `cluster_name` | `"es-cluster"` | Name of the Elasticsearch cluster. |
| `es_user` | `"admin"` | Administrative username created on bootstrap. |
| `es_password` | `"password123"` | Password for the administrative user. |

### Security & TLS

| Variable | Default | Description |
|----------|---------|-------------|
| `tls_enable` | `true` | Enable/Disable encryption for Transport and HTTP layers. |
| `push_certs` | `true` | Set to `false` if certificates are already on the server (provided by your organization). |
| `es_security_enabled` | `true` | Enable built-in security (Basic Auth). |
| `key_name` | `"node1.key"` | Filename of the node private key. |
| `certificate_name` | `"node1.crt"` | Filename of the node certificate. |
| `certificate_authorities_name` | `"ca.crt"` | Filename of the CA certificate. |

### Networking & Discovery

| Variable | Default | Description |
|----------|---------|-------------|
| `http_port` | `9200` | Port for client communication. |
| `tcp_port` | `9300` | Port for inter-node transport. |
| `es_seed_hosts_group` | `"esm"` | Inventory group used for discovery (master nodes). |

## Usage

### 1. Configure your Inventory
Define your nodes in groups to assign roles:
```ini
[es]
node1 ansible_host=<ip>
node2 ansible_host=<ip>
node3 ansible_host=<ip>

[esm]
node1
node2
node3
```
### 2. Set your Credentials (`group_vars/all.yml`)
```yaml
es_user: "elasticsearch-opstree"
es_password: "elasticsearch0610"
push_certs: false  # Use organization certs already on the server
```

### 3. Run the Playbook
```bash
ansible-playbook -i inventory.ini deploy.yml
```

## Accessing the Cluster

Once deployed, you can verify the cluster health from any node:

**If TLS is Enabled:**
```bash
curl -k -u elasticsearch-opstree:elasticsearch0610 https://localhost:9200/_cluster/health?pretty
```

**If TLS is Disabled:**
```bash
curl -u elasticsearch-opstree:elasticsearch0610 http://localhost:9200/_cluster/health?pretty
```

## Author
Originally by Opstree Solutions. 
Upgraded and modernized for Elasticsearch 9.x production standards.

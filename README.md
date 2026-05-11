# Elasticsearch Cluster Ansible Role (9.x)

This Ansible role automates the deployment of a secure, production-ready Elasticsearch 9.x cluster. It is designed to handle multi-node architectures with built-in support for TLS encryption, automated user management, and OS-level performance tuning.

## Key Features

- **Modern Standards**: Fully compliant with **FQCN** (Fully Qualified Collection Names) for long-term maintainability.
- **Secure by Default**: 
  - Automated **Transport and HTTP TLS/SSL** configuration.
  - Support for **externally provided certificates** (organization-issued) via the `elasticsearch_push_certs` flag.
  - **Automated User Bootstrapping**: Automatically creates a custom administrative user with configurable roles.
- **Robust Discovery**: Intelligent bootstrapping logic for `discovery.seed_hosts` and `cluster.initial_master_nodes` that handles both new clusters and single-node setups.
- **Performance Optimized**: Automatically configures critical OS settings (`vm.max_map_count`, `swappiness`, PAM limits) and manages JVM heap allocation.
- **Automated Backup (Minio/S3)**: 
  - Integrated support for **S3-compatible** storage (Minio).
  - Securely manages credentials using the **Elasticsearch Keystore**.
<<<<<<< HEAD
  - Automated **SLM (Snapshot Lifecycle Management)** policies for daily backups and retention (cleanup).
- **Multi-OS Support**: Optimized for Ubuntu 20.04/22.04, Debian 11/12, and RHEL 8/9.
=======
  - Automated **SLM (Snapshot Lifecycle Management)** policies for daily backups and retention.
- **Multi-OS Support**: Optimized for Ubuntu, Debian, and RHEL-based systems.
>>>>>>> 7560f46 (added)

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

Variables are defined in `defaults/main.yml`. Prefix all overrides with `elasticsearch_`.

### Core Configuration

| Variable | Default | Description |
|----------|---------|-------------|
| `elasticsearch_version` | `"9.3.4"` | The exact version of Elasticsearch to install. |
| `elasticsearch_cluster_name` | `"es-cluster"` | Name of the Elasticsearch cluster. |
| `elasticsearch_user` | `"admin"` | Administrative username created on bootstrap. |
| `elasticsearch_password` | `"password123"` | Password for the administrative user. |
| `elasticsearch_user_roles` | `["superuser"]` | List of roles assigned to the administrative user. |
| `elasticsearch_home` | `"/usr/share/elasticsearch"` | Installation home directory. |
| `elasticsearch_conf_path` | `"/etc/elasticsearch"` | Configuration directory (without trailing slash). |

### Security & TLS

| Variable | Default | Description |
|----------|---------|-------------|
| `elasticsearch_tls_enable` | `true` | Enable/Disable encryption for Transport and HTTP layers. |
| `elasticsearch_push_certs` | `true` | Set to `false` if certificates are already on the server. |
| `elasticsearch_key_name` | `"node1.key"` | Filename of the node private key. |
| `elasticsearch_certificate_name` | `"node1.crt"` | Filename of the node certificate. |
| `elasticsearch_ca_name` | `"ca.crt"` | Filename of the CA certificate. |

### Backup & Snapshots (Minio/S3)

| Variable | Default | Description |
|----------|---------|-------------|
<<<<<<< HEAD
| `http_port` | `9200` | Port for client communication. |
| `tcp_port` | `9300` | Port for inter-node transport. |

### Backup & Snapshots (Minio/S3)

| Variable | Default | Description |
|----------|---------|-------------|
| `es_backup_enabled` | `true` | Enable automated Minio/S3 backup integration. |
| `es_minio_endpoint` | `""` | The S3 API endpoint for Minio (e.g., `minio.example.com:9000`). |
| `es_minio_access_key` | `""` | Minio Access Key (Sensitive). |
| `es_minio_secret_key` | `""` | Minio Secret Key (Sensitive). |
| `es_minio_bucket` | `""` | The bucket name in Minio for storing snapshots. |
| `es_snapshot_repo_name` | `"minio_backup"` | The internal name for the snapshot repository. |
| `es_seed_hosts_group` | `"esm"` | Inventory group used for discovery (master nodes). |
=======
| `elasticsearch_backup_enabled` | `true` | Enable automated Minio/S3 backup integration. |
| `elasticsearch_minio_endpoint` | `""` | The S3 API endpoint for Minio. |
| `elasticsearch_slm_policy_name` | `"daily-snapshots"` | Name of the SLM policy. |
| `elasticsearch_snapshot_repo_name` | `"minio_backup"` | Snapshot repository name. |
>>>>>>> 7560f46 (added)

## Usage

### 1. Set your Credentials (`group_vars/all.yml`)
```yaml
<<<<<<< HEAD
es_user: ""
es_password: ""
push_certs: false  # Use organization certs already on the server
=======
elasticsearch_user: "prod-admin"
elasticsearch_password: "secure-password"
elasticsearch_user_roles:
  - superuser
  - kibana_system
>>>>>>> 7560f46 (added)
```

### 2. Run the Playbook
```bash
ansible-playbook -i inventory.ini deploy.yml
```

## Accessing the Cluster

**If TLS is Enabled:**
```bash
curl -k -u username:password https://localhost:9200/_cluster/health?pretty
```

## Disaster Recovery (Restore)

To restore data from Minio, explicitly enable the restore task:
```bash
<<<<<<< HEAD
curl -u username:password http://localhost:9200/_cluster/health?pretty
=======
ansible-playbook -i inventory.ini deploy.yml -e "elasticsearch_restore_enable=true"
>>>>>>> 7560f46 (added)
```

## Author
Modernized for Elasticsearch 9.x production standards by Antigravity.
Originally by Opstree Solutions.

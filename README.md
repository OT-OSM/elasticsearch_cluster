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
  - Automated **SLM (Snapshot Lifecycle Management)** policies for daily backups and retention.
- **Multi-OS Support**: Optimized for Ubuntu, Debian, and RHEL-based systems.

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
| `elasticsearch_backup_enabled` | `true` | Enable automated Minio/S3 backup integration. |
| `elasticsearch_minio_endpoint` | `""` | The S3 API endpoint for Minio. |
| `elasticsearch_slm_policy_name` | `"daily-snapshots"` | Name of the SLM policy. |
| `elasticsearch_snapshot_repo_name` | `"minio_backup"` | Snapshot repository name. |
| `elasticsearch_backup_schedule` | `"0 0 0 * * ?"` | **RPO**: Snapshot frequency (Cron expression). |
| `elasticsearch_backup_retention_days` | `"30d"` | **RPO**: How long to keep snapshots. |
| `elasticsearch_backup_max_count` | `50` | **RPO**: Maximum number of snapshots to keep. |

## Usage

### 1. Set your Credentials (`group_vars/all.yml`)
```yaml
elasticsearch_user: "prod-admin"
elasticsearch_password: "secure-password"
elasticsearch_user_roles:
  - superuser
  - kibana_system
```

### 2. Run the Playbook
```bash
ansible-playbook -i inventory.ini deploy.yml
```

## Accessing & Verifying the Cluster

Below are the key URLs and API endpoints to verify the cluster health, node membership, master coordination, and exporter metrics. Replace `<node-ip>` with any of your cluster node IPs.

### 1. Elasticsearch API Verifications (Port 9200)

* **Cluster Health Status**
  Check the overall health, status (green/yellow/red), and the active node count:
  * **URL**: `http://<node-ip>:9200/_cluster/health?pretty`
  * **Command**:
    ```bash
    curl -s http://<node-ip>:9200/_cluster/health?pretty
    ```

* **Node Membership & Roles**
  See all cluster nodes, resource usage, active roles, and identify the active master node (marked with a `*`):
  * **URL**: `http://<node-ip>:9200/_cat/nodes?v`
  * **Command**:
    ```bash
    curl -s "http://<node-ip>:9200/_cat/nodes?v"
    ```

* **Active Master Node**
  Directly fetch the IP and details of the current elected master node:
  * **URL**: `http://<node-ip>:9200/_cat/master?v`
  * **Command**:
    ```bash
    curl -s "http://<node-ip>:9200/_cat/master?v"
    ```

* **Shard Allocation**
  View how indexes and shards (primary and replica) are distributed across nodes:
  * **URL**: `http://<node-ip>:9200/_cat/shards?v`
  * **Command**:
    ```bash
    curl -s "http://<node-ip>:9200/_cat/shards?v"
    ```

*Note: If TLS is enabled (`elasticsearch_tls_enable: true`), use `https` instead of `http` and pass authentication credentials (e.g., `curl -k -u username:password https://...`).*

### 2. Prometheus Exporter Metrics (Port 9114)

The Prometheus exporter runs as a dedicated systemd service on port `9114` of each node to expose raw metrics for Prometheus collection:
* **Metrics Endpoint URL**: `http://<node-ip>:9114/metrics`
* **Command**:
  ```bash
  curl -s http://<node-ip>:9114/metrics
  ```

## Disaster Recovery (Restore)

To restore data from Minio, explicitly enable the restore task:
```bash
ansible-playbook -i inventory.ini deploy.yml -e "elasticsearch_restore_enable=true"
```

## RTO & RPO Configuration

The role is designed to meet business-defined **Recovery Point Objectives (RPO)** and **Recovery Time Objectives (RTO)**:

### 1. RPO (Recovery Point Objective)
Control data loss by adjusting backup frequency and retention in `group_vars/all.yml`:
- **Backup Frequency**: Change `elasticsearch_backup_schedule` (e.g., to `0 0 * * * ?` for 1-hour RPO).
- **Retention Policy**: Adjust `elasticsearch_backup_retention_days` and `elasticsearch_backup_max_count` to meet compliance requirements.

### 2. RTO (Recovery Time Objective)
Minimize downtime during a disaster:
- **Automation**: Use the built-in `restore.yml` tasks to automate the recovery process.
- **Selective Restore**: Use `elasticsearch_restore_indices` to prioritize critical data first.
- **Prefixing**: Restored indices are prefixed with `restored-` (configurable via `elasticsearch_restore_prefix`) to allow verification before cutting over.

## Author
**Sneha Joshi**

Originally by Opstree Solutions.

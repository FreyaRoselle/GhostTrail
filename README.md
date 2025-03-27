# GhostTrail - Log Editor

GhostTrail is an advanced **log file editor** for **Kali Linux**, built in **C**. It allows users to **view, modify, delete, and search system logs** dynamically. Ideal for system administrators, security researchers, and penetration testers.

## 🔥 Features
- **View system logs** with line numbers.
- **Modify log entries** with new content.
- **Delete specific log lines** securely.
- **Search logs for keywords** to find relevant events.
- **Backup system** ensures original logs are preserved.
- **Works with multiple log files**, including:
  - `/var/log/auth.log`
  - `/var/log/syslog`
  - `/var/log/apache2/access.log`
  - And more...

## 🚀 Installation & Usage

### 1️⃣ **Compile GhostTrail**
```sh
gcc ghosttrail.c -o ghosttrail

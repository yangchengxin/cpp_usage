# 🚀 Cpp_Usage

使用 C++ 构建的高性能基础组件库，包含线程池、无锁环形缓冲区等常用工具，适用于多线程应用开发。

---

## ✨ 特性

| 模块 | 描述 | 适用场景 |
|------|------|----------|
| **ThreadPool** | 高性能线程池，支持任务调度与并发执行 | 多线程任务处理、异步计算 |
| **RingBuffer** | 无锁单生产者消费者环形缓冲区（原子操作） | 高性能生产者消费者场景 |
| **RingBufferV2** | 带锁环形缓冲区（互斥锁+条件变量） | 多生产者多消费者场景 |
| **TodoStateMachine** | TODO 任务状态机（State Pattern） | 任务状态管理、工作流控制 |

---

## 📦 项目结构

```
Cpp_Usage
├── threadpool/          # 线程池实现
│   ├── include/
│   │   └── ThreadPool.hpp
│   ├── main.cpp
│   └── CMakeLists.txt
│
├── ringbuffer/         # 无锁环形缓冲区 (v1)
│   ├── include/
│   │   └── ringbuffer.hpp
│   ├── src/
│   │   └── ringbuffer.cc
│   ├── main.cc
│   └── CMakeLists.txt
│
├── ringbufferv2/       # 带锁环形缓冲区 (v2)
│   ├── include/
│   │   └── ringbufferv2.hpp
│   ├── src/
│   │   └── ringbufferv2.cc
│   ├── test_ringbufferv2.cc
│   └── CMakeLists.txt
│
├── todostatemachine/   # TODO 状态机
│   ├── include/
│   │   └── todostatemachine.hpp
│   ├── src/
│   │   └── todostatemachine.cc
│   ├── test_todostatemachine.cc
│   └── CMakeLists.txt
│
├── CMakeLists.txt      # 根目录构建配置
└── README.md
```

---

## 🛠️ 快速开始

### 编译构建

```bash
# 创建构建目录
mkdir build && cd build

# 配置项目
cmake ..

# 编译
cmake --build .
```

### 运行示例

```bash
# 运行线程池示例
./threadpool/threadpool

# 运行 RingBuffer 示例
./ringbuffer/ringbuffer

# 运行 RingBufferV2 测试
./ringbufferv2/test_ringbufferv2

# 运行 TODO 状态机测试
./todostatemachine/test_todostatemachine
```

---

## 📝 模块说明

### ThreadPool

- **接口**: `threadpool(size_t n)` - 创建 n 个工作线程
- **方法**: `enqueue(F&& f, Args&&... args)` - 添加任务，返回 `std::future`
- **特点**: 线程安全、支持返回值、自动管理生命周期

### RingBuffer (无锁版本)

- **接口**: `RingBuffer<T>(size_t capacity)` - 创建指定容量的缓冲区
- **方法**: 
  - `push(const T& item)` - 写入元素
  - `pop()` - 读取元素，返回 `std::optional<T>`
  - `size()`, `empty()`, `full()` - 状态查询
- **特点**: 单生产者单消费者、无锁实现（原子操作）

### RingBufferV2 (带锁版本)

- **接口**: `RingBufferV2<T>(size_t capacity)` - 创建指定容量的缓冲区
- **方法**: 同 RingBuffer
- **特点**: 多生产者多消费者、线程安全、阻塞等待

### TodoStateMachine (状态机)

- **状态**: `TODO` → `IN_PROGRESS` → `DONE` / `CANCELLED`
- **接口**: 
  - `TodoContext(title)` - 创建任务上下文
  - `start()` - 开始处理 (TODO → IN_PROGRESS)
  - `complete()` - 完成 (IN_PROGRESS → DONE)
  - `cancel()` - 取消 (TODO/IN_PROGRESS → CANCELLED)
  - `reopen()` - 重新打开 (DONE/CANCELLED → TODO)
- **特点**: 状态模式实现、合法状态转换、拒绝非法转换

---

## 🔄 状态转换图

```
       ┌─────────┐     start      ┌─────────────┐
       │  TODO   │ ─────────────► │ IN_PROGRESS │
       └─────────┘               └─────────────┘
            ▲                          │
            │                          │
            │        reopen            │
            └──────────────────────────┘
                  ▲              ▲
                  │   complete   │   cancel
       ┌──────────┴───┐   ┌─────┴──────────┐
       │     DONE     │   │   CANCELLED    │
       └──────────────┘   └───────────────┘
                  │              ▲
                  │    start     │
                  └──────────────┘
```

---

## 📄 许可证

MIT License

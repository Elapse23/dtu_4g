# SPI Flash 驱动命名简化对照表

## 命名简化完成总结

### 🎯 简化原则
- **去除冗余前缀**：`sFLASH_` → `FLASH_`
- **简化GPIO描述**：`GPIO_PORT` → `PORT`, `GPIO_CLK` → `CLK`
- **简化函数命名**：`spi_flash_` → `bsp_flash_`，`drv_` → `Flash_`
- **统一命名风格**：保持一致的命名约定

### 📝 头文件 (bsp_spiflash.h) 修改对照

| 修改前 | 修改后 | 说明 |
|--------|--------|------|
| `sFLASH_SPI` | `FLASH_SPI` | SPI外设定义 |
| `sFLASH_SPI_CLK` | `FLASH_SPI_CLK` | SPI时钟 |
| `sFLASH_SPI_SCK_PIN` | `FLASH_SCK_PIN` | SCK引脚 |
| `sFLASH_SPI_SCK_GPIO_PORT` | `FLASH_SCK_PORT` | SCK端口 |
| `sFLASH_SPI_SCK_GPIO_CLK` | `FLASH_SCK_CLK` | SCK时钟 |
| `sFLASH_SPI_MISO_PIN` | `FLASH_MISO_PIN` | MISO引脚 |
| `sFLASH_SPI_MISO_GPIO_PORT` | `FLASH_MISO_PORT` | MISO端口 |
| `sFLASH_SPI_MISO_GPIO_CLK` | `FLASH_MISO_CLK` | MISO时钟 |
| `sFLASH_SPI_MOSI_PIN` | `FLASH_MOSI_PIN` | MOSI引脚 |
| `sFLASH_SPI_MOSI_GPIO_PORT` | `FLASH_MOSI_PORT` | MOSI端口 |
| `sFLASH_SPI_MOSI_GPIO_CLK` | `FLASH_MOSI_CLK` | MOSI时钟 |
| `sFLASH_CS_PIN` | `FLASH_CS_PIN` | 片选引脚 |
| `sFLASH_CS_GPIO_PORT` | `FLASH_CS_PORT` | 片选端口 |
| `sFLASH_CS_GPIO_CLK` | `FLASH_CS_CLK` | 片选时钟 |
| `SPI_FLASH_PAGE_SIZE` | `FLASH_PAGE_SIZE` | 页大小 |
| `SPI_FLASH_SECTOR_SIZE` | `FLASH_SECTOR_SIZE` | 扇区大小 |
| `SPI_FLASH_BLOCK_SIZE` | `FLASH_BLOCK_SIZE` | 块大小 |
| `SPIFLASH_CS_LOW()` | `FLASH_CS_LOW()` | 片选拉低宏 |
| `SPIFLASH_CS_HIGH()` | `FLASH_CS_HIGH()` | 片选拉高宏 |
| `CMD_READ_REG` | `CMD_READ_STATUS` | 读状态寄存器命令 |
| `CMD_WRITE_EN` | `CMD_WRITE_ENABLE` | 写使能命令 |
| `bsp_spi_flash_init()` | `bsp_flash_init()` | 初始化函数 |

### 📝 源文件 (bsp_spiflash.c) 修改对照

| 修改前 | 修改后 | 说明 |
|--------|--------|------|
| `sFLASH_LowLevel_DeInit()` | `Flash_LowLevel_DeInit()` | 底层反初始化 |
| `sFLASH_LowLevel_Init()` | `Flash_LowLevel_Init()` | 底层初始化 |
| `sFLASH_DeInit()` | `Flash_DeInit()` | 反初始化 |
| `spi_flash_init()` | `bsp_flash_init()` | 初始化函数 |
| `flash_write_byte()` | `bsp_flash_write_byte()` | 字节写入 |
| `flash_read_byte()` | `bsp_flash_read_byte()` | 字节读取 |
| `drv_flash_read()` | `Flash_Read()` | 数据读取 |
| `drv_flash_write_enable()` | `Flash_WriteEnable()` | 写使能 |
| `drv_flash_wait_for_idle()` | `Flash_WaitForIdle()` | 等待空闲 |
| `drv_flash_erase_chip()` | `Flash_EraseChip()` | 芯片擦除 |
| `drv_flash_erase_sector()` | `Flash_EraseSector()` | 扇区擦除 |
| `drv_flash_write_page()` | `Flash_WritePage()` | 页写入 |
| `drv_flash_write_buffer()` | `Flash_WriteBuffer()` | 缓冲区写入 |

### 🔧 改进亮点

1. **命名一致性**：
   - 统一使用 `FLASH_` 前缀替代 `sFLASH_` 和 `SPI_FLASH_`
   - GPIO相关简化为 `PORT` 和 `CLK`
   - 函数命名采用一致的风格

2. **可读性提升**：
   - 移除冗余的 `SPI_` 前缀（因为已在Flash上下文中）
   - 简化宏定义名称，更直观易懂
   - 函数名称更简洁明了

3. **维护性增强**：
   - 减少命名长度，降低输入错误概率
   - 统一的命名约定便于团队开发
   - 清晰的层次结构（BSP层、驱动层）

### 🎨 命名规范

**宏定义规范：**
- 硬件相关：`FLASH_` + `功能` + `_类型`
- 命令相关：`CMD_` + `功能`

**函数命名规范：**
- BSP层：`bsp_flash_` + `功能`
- 驱动层：`Flash_` + `功能`（采用驼峰命名）

**注释规范：**
- 使用中文注释，提高可读性
- 保持注释与代码的一致性

### ✅ 验证清单

- [x] 头文件宏定义全部简化
- [x] 源文件函数名全部简化
- [x] 宏调用全部更新
- [x] 注释内容同步更新
- [x] 编码风格保持一致
- [x] 功能完整性保持不变

### 🔄 兼容性说明

此次重命名属于破坏性更新，使用该驱动的其他代码需要同步更新：

1. **包含头文件的源文件**需要更新函数调用
2. **配置文件**需要更新相关宏定义引用
3. **构建脚本**需要确认文件路径正确

### 📚 使用示例

```c
// 初始化Flash
bsp_flash_init();

// 读取数据
uint8_t data[256];
Flash_Read(data, 0x1000, 256);

// 擦除扇区
Flash_EraseSector(0x1000);

// 写入数据
Flash_WriteBuffer(data, 0x1000, 256);
```

这次简化命名大大提高了代码的可读性和维护性，为后续开发提供了更好的基础。

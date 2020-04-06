### token生成方法

`man 5 machine-id`  
> The /etc/machine-id file contains the unique machine ID of the local system that is set during installation or boot. The machine ID is a single newline-terminated, hexadecimal, 32-character, lowercase ID. When decoded from hexadecimal, this corresponds to a 16-byte/128-bit value. This ID may not be all zeros.

token需求：
* 基于`machine-id`
* 可刷新，即同一`machine-id`可以生成不同的token
* 可根据token判断是否属于`machine-id`，刷新时作为校验

生成过程
* agent提交`machine-id`作为原始数据，以hex编码格式读取
* storage使用`AES-256-CFB`进行加密，加密用到的key为storage配置文件中的`secret`，iv使用node.js的`crypto.randomBytes(16)`随机生成16字节buffer，加密结果保存为buffer
* 将iv的16字节buffer与加密结果拼接，使用base64编码得到token

（最终token需要从buffer转换为可见字符，方便写入配置文件）  
（base64编码与hex相比可以减小token长度）
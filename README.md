# stc89c52-learning

> Clean Public Snapshot Candidate · R1 internal candidate · no remote configured

保留自主技术文档以及环境与时钟信息终端的独立 practice 实现。

## Public snapshot scope

- 保留独立源码文件：6 个；范围以当前 candidate tree 为准。
- 课程源码、课程图片、PDF、未知字体、未知生成资产和不必要的第三方/vendor 大包均不在本快照中。
- $(System.Collections.Hashtable.Limitation)

## Contents

- [技术文档](docs/)
- [自主架构图来源说明](assets/images/SOURCES.md)
- [环境与时钟信息终端 practice](projects/13_环境与时钟信息终端/)

## Validation boundary

- 未提供板端运行证据时，不声称 hardware verified。
- 未执行真实构建时，不声称 build verified。
- 文档中的协议、地址、寄存器和架构关系属于技术事实说明，不表示未知来源的具体实现已被保留。

## License scope

根目录 LICENSE 仅适用于该 candidate 中由仓库维护者独立编写的文档、图示和代码。未捆绑的 upstream 组件、课程材料和第三方实现不因文档引用而受到根 MIT License 覆盖。
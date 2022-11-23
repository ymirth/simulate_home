1.	允许用户添加房间
2.	允许用户设置房间属性包括：
      1)	窗户的面积
      2)	墙体的面积
      3)	房间的 面积，容积
3.	若为单方间模式，设置单房间的仪器；若为多房屋模式，设置房屋的总的仪器
      1)	可选择设备类型： 空调，新风，加湿器，除湿器，地热
      2)	可选择设备型号
      3)	导入设备数据
4.	环境生成
      1)	使用抽象工厂的模式导入南北方模型
      2)	可选择南北方模型，之后在选择细化的四季模型
      3)	根据模型数据拟合空气的温度曲线
      4)	导入空气的基本属性，co2浓度，温度，风速
5.	状态模拟 以及 图形化显示
      1)	根据输入的设备，计算出房间/房屋，总的：
      制冷量，制热量，通风量，除湿量，加湿量（每秒）
      实时的计算房间温度，湿度，co3浓度。
      2)	实时将数据写入文本本文件，从而使用qt拟合生成房间各个属性的数据图
      3)	显示拟合后的外界空气温度的曲线图
6.	所有设备智能控制 与 功率的合理分配
      1)	根据房间/房屋温度的大小，实时的判断应该的制冷制热模式，从而实现温度的智能控制（空调，地热）空调、地热的联动
      2)	根据空气湿度，判断加湿器，除湿器的开/关，从而实现湿度的智能控制。
      3)	根据co2浓度，判断新风的开/关，从而实现湿度的智能控制。		
      4)	根据环境状态智能的开关设备，从而达到功率的合理分配
7.	多设备联动
      1)	房屋的温度由，输入新风量的温度影响 +空调的温度影响 共同完成，达到指定条件后，空调智能的关闭，从而实现 新风对空调的调控
      2)	加湿器与除湿器 通过房间湿度， 动态的联动， 从而湿度房间相对湿度保持在适宜的40%~50%之间。
      3)	空调地热， 在制热时联动，达到适宜温度后，可以智能的停止联动
8.	多空间需求
      单个空间时可以的设置空间参数以及设备
      多个空间时，可以创建多个空间类，设置基本参数进行累加，给与房间初始化的参数，之后，同一的添加设备，记为房屋总的设备，利于管理

9.	数据模型的建立
      1)	设备数据
      -	数据来源
      所有数据均摘自品牌官网，
      空调：远大科技集团
      新风，除湿器，：松下
      加湿器：电商平台
      地热：互联网
      -	数据存储
      文本文件
      -	数据导入：
      使用单独的一个类记录所有设备的数据，每一种设备数据记录在对应类中，存储与类似unordered_map<string, device{abstract} *>类的哈希表中，				哈希表中存储指针，避免拷贝，节省内存的同时，优化了效率
      -	数据使用
      使用多态处理设备，减少了if-else逻辑判断，降低耦合
      通过一个全局设备数据对象，减少拷贝，以及重复导入
      2)	房屋生成co2速率
      通过互联网查询的单人运动量适中得到的数据
      3)	房屋墙体与窗户
      -	墙体
      墙体有单位热损失率，
      根据墙体面积计算单位热损失率，以及房屋内外温度差计算房屋损失温度
      -	窗户
      根据风速，以及窗户面积计算通风量
      4)	南北方模型，四季模型
      -	数据来源：
      北京，广州 四季的平均温度（高温，低温），平均湿度，平均风速（由月份平均）
      根据 平均高温，低温 使用正太分布区拟合24小时温度，从而生成24小时的温度数据，得到温度曲线
      -	数据存储：文本文件
      -	数据导入：
      抽象工厂模式：南方工厂，北方工厂：分别生产四个季节数据
      季节为环境的子类，减小耦合
      工厂使用单例模式，避免重复导入造成不必要的开销
      -	数据使用：导入到空气模型中
	  5)	房间温度
      - 房间秒换气（新风+窗）后空气的热量	
      - 空调地热，墙体消耗对房间空气热量的该变量
	  6)	房间湿度
      - 相对度以及绝对湿度的转换
      - 房间度的计算：房间内原有空气（减掉新风排出量）水量+ 新风输入空气含水量+单位时间加湿量-单位时间除湿量
	  7)	房间co2浓度
      - 根据阅资料（https://zhuanlan.zhihu.com/p/132539454）建立理想状态模型：
		建立碳守恒方程 :二氧化碳浓度= （（原有空气-新风量）的二氧化碳量+ 新风量中的二氧化碳量，+生成的二氧化碳量）/空间中空气体积

__具体运行结果见结果报告.docx__

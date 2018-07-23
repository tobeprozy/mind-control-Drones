/ *
 mind control robot 
 作者：prozy
 日期：2018年7月23日
 * /

//包括

＃包括 < Servo.h >
＃包括 < SoftwareSerial.h >

＃定义 DEBUGOUTPUT  1
伺服伺服左; //设置左伺服
伺服伺服; //设置正确的伺服
int ledPin = 5 ; //将引脚初始化为引脚5
int ledPin2 = 11 ; //初始化另一个导致11针

byte payloadData [ 256 ] = { 0 }; //将payloadData初始化为0
byte generatedChecksum = 0 ; //将generatedCheckcum初始化为0
字节校验和= 0 ; //将校验和初始化为0
int payloadLength = 0 ; //将payloadLength初始化为0
// byte payloadData [256] = {0};
byte poorQuality = 0 ; //将poorQuality初始化为0
字节注意= 0 ; //将注意力初始化为0
byte meditation = 0 ; //将冥想初始化为0
int maxAttention = 0 ; //将maxAttention初始化为0
int servoUnit = 0 ; //初始化servoUnit为0
int moveUp = 0 ; //将moveUp初始化为0

//系统变量
long lastReceivedPacket = 0 ;
boolean bigPacket = false ;
int bluetoothTx = 2 ;  //蓝牙伴侣的TX-O引脚，Arduino D2
int bluetoothRx = 3 ;  // RX-I引脚的蓝牙伴侣，Arduino D3
int batteryLevel = 0 ;

SoftwareSerial 蓝牙（bluetoothTx，bluetoothRx）;

void  setup（）
{
  //   servoRight.attach（A0）;
  //   servoLeft.attach（A1）;
  pinMode（ledPin，OUTPUT）;
  串行。开始（9600）;  //以9600bps开始串行监视器
  蓝牙。开始（57600）;  //在576000开始蓝牙序列
  延迟（50）;
  蓝牙。写（0x0B）;
}
byte ReadOneByte（）{
  int byteRead;

  而（！蓝牙。可用（））{
    //     Serial.print（“等待蓝牙...... \ n”）;
  }
  byteRead =蓝牙。read（）;

  //   Serial.print（（char）ByteRead）; //回显USB串口的相同字节（用于调试目的）
  //   Serial.print（“\ n”）;

  return byteRead;
}


void  loop（）{
  //   Serial.print（“in loop \ n”）;
  ledDisplay（）; //调用led方法

  而（1）{
    if（ReadOneByte（）！= 170）{
      继续 ;
    }
    if（ReadOneByte（）！= 170）{
      继续 ;
    }
    打破 ;
  }
  // Serial.print（“ready read read length：”）;
  payloadLength = ReadOneByte（）;
//   while（payloadLength == 170）
//   {
//     payloadLength = ReadOneByte（）;
//   }
  //   Serial.print（“Pay load length =”）;
  //   Serial.println（payloadLength，DEC）;
  // Serial.println（payloadLength，DEC）;
  if（payloadLength> 169）{                  //有效载荷长度不能大于169
    //     Serial.print（“有效负载长度：”）; a
    //     Serial.println（（int）payloadLength）;
    回归 ;
  }

  generatedChecksum = 0 ; //将数组某个部分的字节总和设置为0
  for（int i = 0 ; i <payloadLength; i ++）{
    int data = ReadOneByte（）;
    payloadData [i] = data; // ReadOneByte（）; //将有效负载读入内存
    generatedChecksum + = payloadData [i]; //生成的校验和将增加，直到它等于payloadLength
    //     Serial.print（payloadData [i]，HEX）;
    //     Serial.print（“，”）;
  }
  //   Serial.print（“\ n”）;
  checksum = ReadOneByte（）;         //从流中读取校验和字节
  generatedChecksum = 255 -  generatedChecksum;
  //   generatedChecksum＆= 0xFF;
  //   generatedChecksum = ~generatedChecksum＆0xFF;
  if（checksum！= generatedChecksum）{
    //     Serial.println（“错误的校验和”）;
    回归 ;
  }
  if（checksum == generatedChecksum）{ //最近读取的校验和必须等于生成的校验和
    // Serial.print（“Checksum working \ n”）;
    for（int i = 0 ; i <payloadLength; i ++）{ //直到i =有效载荷的长度，执行循环
      switch（payloadData [i]）{ //将256切换为i值
        case  0x01：//对应耳机动作的特定十六进制值（电池电量）
          我++;
          batteryLevel = payloadData [i];
          串行。打印（“ BATTERY LEVEL = ”）;
          串行。println（batteryLevel，DEC）;
          打破 ;
        case  0x2：//（信号质量）
          我++;
          poorQuality = payloadData [i];
          bigPacket = true ;
          串行。print（“ poorQuality = ”）;
          串行。println（poorQuality，DEC）;
          打破 ;
        case  0x4：//注意
          我++;
          attention = payloadData [i];
          ledDisplay（）; //调用led方法，显示注意值
          串行。print（“ attention = ”）; //显示注意力值
          串行。println（注意，DEC）;
          moveServo（）; //呼叫伺服方法
          打破 ;
        案例 0x5：//冥想
          我++;
          meditation = payloadData [i];
          ledDisplay（）; //调用led方法，显示冥想值
//           digitalWrite（9，LOW）;
          串行。print（“ meditation = ”）;
          串行。println（冥想，DEC）;
          moveServo（）; //目前是 unnessecary，因为机器人只会移动到注意力值。
          打破 ;
        case  0x80：//单个big-endian 16位值
          i = i + 3 ;
          打破 ;
        case  0x81：//八个bid-endian 4字节点值代表不同的脑电波
          i = i + 33 ;
          打破 ;
        case  0x83：//表示不同脑电波的八个大端3字节无符号整数值
//           Serial.println（“有脑波”）;
          i = i + 25 ;
          打破 ;
        case  0x86：//两个字节的big-endian无符号整数
          i = i + 3 ;
          打破 ;
        默认值：
          打破 ;
      }
    }
  }
}
void  ledDisplay（）{
  if（注意<= 100 &&注意> 0）//如果注意力小于或等于100且大于0，则新变量attn等于注意值乘以255并除以100
  {
      INT收件人=注意* 255个 / 100 ;
//     Serial.print（“attn =”）;
//     Serial.println（attn）;
    analogWrite（ledPin，attn）; //黄色LED显示价值
//     延迟（100）;
//     analogWrite（ledPin，0）;
  }
  if（meditation <= 100 && meditation> 0）//如果冥想值小于或等于100且大于0，则新变量medit =冥想乘以255并除以100，然后加入冥想时间2
  {
    INT medit =（冥想* 255个 / 100）+（冥想* 2）;
    analogWrite（ledPin2，medit）; // led显示中介值
  }
    
}

void  moveServo（）{
  if（注意> 100 ||注意<= 0）{ //如果注意力小于100且大于或等于0，则返回该值。
    回归 ;
  }
  if（注意> maxAttention）{ //如果注意力ids大于最大注意力，则执行条件
    // digitalWrite（ledPin2,100）; //绿色LED显示注意力超过最大注意力
    moveUp =（注意 -  maxAttention）/ 10 ; //变量moveUp是机器人向上移动的段数。这是通过当前最大注意力值与您刚刚收到的新最大值之间的空间量来计算的。对于一位数字，这两者之间的间距除以10。
    maxAttention =注意; //新的最大注意力值
    串行。print（“ moveUp：”）;
    串行。println（moveUp）;
    servoLeft。附上（A0）; //开始移动伺服
    servoRight。附（A1）; //开始移动伺服
    servoLeft。写（180）; //伺服方向（向后）
    servoRight。写（0）; //伺服方向（前进）
    延迟（moveUp * 703）; //时间伺服移动 机器人需要703毫秒才能移动一段（轮子直径的一半）。此时间乘以要移动的段数（moveUp）。
    servoLeft。detach（）; //停止伺服
    servoRight。detach（）; //停止伺服
  }
  

}




//
//
//   if（bigPacket）{
//     if（poorQuality == 0）
//     {}
//     别的
//     Serial.print（“PoorQuality：”）;
//     Serial.print（poorQuality，DEC）;
//     Serial.print（“注意：”）;
//     Serial.print（注意，DEC）;
//     Serial.print（“自上次数据包以来的时间：”）;
//     Serial.print（millis（） -  lastReceivedPacket，DEC）;
//     lastReceivedPacket = millis（）;
//     Serial.print（“\ n”）;
//   }
//     //将蓝牙打印的任何字符发送到串行监视器
//     Serial.print（（char）bluetooth.read（））;
//   }
//         bigPacket = false;
//       }
//      }
//
// // if（Serial.available（））//如果在串行监视器中输入了东西
// // {
// // //将串行监视器打印的任何字符发送到蓝牙
// // bluetooth.print（（char）Serial.read（））;
// //}
//   //并永远循环！

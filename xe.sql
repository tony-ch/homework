-- MySQL dump 10.13  Distrib 5.7.16, for Win64 (x86_64)
--
-- Host: localhost    Database: xe
-- ------------------------------------------------------
-- Server version	5.7.16-log

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `admin`
--

DROP TABLE IF EXISTS `admin`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `admin` (
  `LOGINNAME` varchar(30) NOT NULL,
  `NAME` varchar(30) DEFAULT NULL,
  `PASSWORD` varchar(30) DEFAULT NULL,
  PRIMARY KEY (`LOGINNAME`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `admin`
--

LOCK TABLES `admin` WRITE;
/*!40000 ALTER TABLE `admin` DISABLE KEYS */;
INSERT INTO `admin` VALUES ('lileixiao','????','llx123'),('xinghongmei','adssfd','asdf'),('zhangliu','sss','ss'),('zhangqi','sddfsf','sdfsdf'),('zhangsan','????','zs123'),('zhangsanfeng','zzzz','123'),('zhangsanliang','123','123'),('zhangsi','zhangaaaa','123'),('zhangwuhu','aaaa','aaa');
/*!40000 ALTER TABLE `admin` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `classtbl`
--

DROP TABLE IF EXISTS `classtbl`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `classtbl` (
  `CLASSNO` varchar(30) NOT NULL,
  `CLASSNAME` varchar(30) DEFAULT NULL,
  `COLLEGE` varchar(50) DEFAULT NULL,
  PRIMARY KEY (`CLASSNO`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `classtbl`
--

LOCK TABLES `classtbl` WRITE;
/*!40000 ALTER TABLE `classtbl` DISABLE KEYS */;
INSERT INTO `classtbl` VALUES ('2012001','140611','edu'),('2012002','???12-2','edu'),('2012003','??2-1','edu'),('2012004','??2-2','edu'),('2012005','???2-1','edu'),('2012006','???2-2','edu'),('2012009','????12','edu'),('3477111','??е12-2','edu');
/*!40000 ALTER TABLE `classtbl` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `course`
--

DROP TABLE IF EXISTS `course`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `course` (
  `COURSENO` varchar(30) NOT NULL,
  `COURSENAME` varchar(50) DEFAULT NULL,
  `STUDYTIME` int(11) DEFAULT NULL,
  `GRADE` int(11) DEFAULT NULL,
  `COURSETYPE` int(11) DEFAULT NULL,
  `TERM` int(11) DEFAULT NULL,
  PRIMARY KEY (`COURSENO`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `course`
--

LOCK TABLES `course` WRITE;
/*!40000 ALTER TABLE `course` DISABLE KEYS */;
INSERT INTO `course` VALUES ('0205010','Java3',40,4,2,1),('0205011','Java4',64,3,2,1),('03030101201','Java1',48,3,2,3),('0305014','Java2',60,3,0,1),('0305015','Java5',60,3,2,2),('0305016','Java6',32,2,2,2),('0305017','Java',56,3,3,3),('0305018','Java7',48,2,3,3),('123321','Java8',32,2,2,5);
/*!40000 ALTER TABLE `course` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `coursearrange`
--

DROP TABLE IF EXISTS `coursearrange`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `coursearrange` (
  `ARRANGENO` varchar(30) NOT NULL,
  `COURSENO` varchar(30) DEFAULT NULL,
  `CLASSNO` varchar(30) DEFAULT NULL,
  `TEACHERNO` varchar(30) DEFAULT NULL,
  `STUDYROOM` varchar(30) DEFAULT NULL,
  PRIMARY KEY (`ARRANGENO`),
  KEY `COURSENO` (`COURSENO`),
  KEY `CLASSNO` (`CLASSNO`),
  KEY `TEACHERNO` (`TEACHERNO`),
  CONSTRAINT `coursearrange_ibfk_1` FOREIGN KEY (`COURSENO`) REFERENCES `course` (`COURSENO`),
  CONSTRAINT `coursearrange_ibfk_2` FOREIGN KEY (`CLASSNO`) REFERENCES `classtbl` (`CLASSNO`),
  CONSTRAINT `coursearrange_ibfk_3` FOREIGN KEY (`TEACHERNO`) REFERENCES `teacher` (`TEACHERNO`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `coursearrange`
--

LOCK TABLES `coursearrange` WRITE;
/*!40000 ALTER TABLE `coursearrange` DISABLE KEYS */;
INSERT INTO `coursearrange` VALUES ('111111','0305015','2012002','3214433','234'),('1222222','0305016','2012009','321232','212'),('201300012','0305014','2012001','9087654','345'),('20141234','03030101201','2012001','2143122','312'),('2014321012','0305017','2012001','2143122','213'),('210122222','0305018','2012001','20123333','321'),('22222222','0305017','2012003','3214433','212'),('45621111','123321','2012001','2143122','212'),('wfdaf','0205010','2012001','2004123','adsf');
/*!40000 ALTER TABLE `coursearrange` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `score`
--

DROP TABLE IF EXISTS `score`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `score` (
  `ID` int(11) NOT NULL,
  `STUDENTNO` varchar(30) DEFAULT NULL,
  `ARRANGENO` varchar(30) DEFAULT NULL,
  `SCORE` int(11) DEFAULT NULL,
  `REMARK` varchar(5) DEFAULT NULL,
  PRIMARY KEY (`ID`),
  KEY `STUDENTNO` (`STUDENTNO`),
  KEY `ARRANGENO` (`ARRANGENO`),
  CONSTRAINT `score_ibfk_1` FOREIGN KEY (`STUDENTNO`) REFERENCES `student` (`STUDENTNO`),
  CONSTRAINT `score_ibfk_2` FOREIGN KEY (`ARRANGENO`) REFERENCES `coursearrange` (`ARRANGENO`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `score`
--

LOCK TABLES `score` WRITE;
/*!40000 ALTER TABLE `score` DISABLE KEYS */;
INSERT INTO `score` VALUES (2,'20120010','wfdaf',13,'A'),(12,'20120010','20141234',13,'A'),(22,'20120010','201300012',0,NULL),(32,'20120010','2014321012',0,NULL),(42,'20120010','210122222',0,NULL),(52,'20120010','45621111',0,NULL),(61,'200202020','22222222',0,NULL);
/*!40000 ALTER TABLE `score` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `student`
--

DROP TABLE IF EXISTS `student`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `student` (
  `STUDENTNO` varchar(30) NOT NULL,
  `NAME` varchar(30) DEFAULT NULL,
  `PASSWORD` varchar(30) DEFAULT NULL,
  `ADDRESS` varchar(100) DEFAULT NULL,
  `PHONE` varchar(50) DEFAULT NULL,
  `EMAIL` varchar(30) DEFAULT NULL,
  `CLASSNO` varchar(30) DEFAULT NULL,
  PRIMARY KEY (`STUDENTNO`),
  KEY `CLASSNO` (`CLASSNO`),
  CONSTRAINT `student_ibfk_1` FOREIGN KEY (`CLASSNO`) REFERENCES `classtbl` (`CLASSNO`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `student`
--

LOCK TABLES `student` WRITE;
/*!40000 ALTER TABLE `student` DISABLE KEYS */;
INSERT INTO `student` VALUES ('200202020','zhang','123','asfadsf','13112344321','aaa@23.com','2012003'),('20120010','????','qzl123','dsds','13304175874','ll@126.com','2012001');
/*!40000 ALTER TABLE `student` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `teacher`
--

DROP TABLE IF EXISTS `teacher`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `teacher` (
  `TEACHERNO` varchar(30) NOT NULL,
  `TEACHERNAME` varchar(30) DEFAULT NULL,
  `PASSWORD` varchar(30) DEFAULT NULL,
  `PROFESSIONAL` int(11) DEFAULT NULL,
  `EDUCATION` varchar(30) DEFAULT NULL,
  `ADDRESS` varchar(100) DEFAULT NULL,
  `PHONE` varchar(50) DEFAULT NULL,
  `EMAIL` varchar(30) DEFAULT NULL,
  `SUBJECT` varchar(50) DEFAULT NULL,
  PRIMARY KEY (`TEACHERNO`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `teacher`
--

LOCK TABLES `teacher` WRITE;
/*!40000 ALTER TABLE `teacher` DISABLE KEYS */;
INSERT INTO `teacher` VALUES ('2004123','韦国华','wym123',0,'null','fuckathome','13274756234','wym123@126.com','21212'),('2005001','李亚梅','111',3,'1','2','3','4@ww.com','5'),('2007001','常涛','ly123',3,'null','null','13123115723','ly123@126.com','tttt'),('20123333','牛瓣','123',1,'null','null','13112345678','ss@123.com','adfdf'),('2143122','????','123',3,'null','null','13212344321','llx@123.com','adsfdf'),('22','hdusd','zs123',1,'ds','dsd','21','gsysga@136.com','eweweewe'),('23','1223','zs123',1,'wqw','wqwqw','12','3732939@qw.com','ewewewe'),('24','zhang','zs123',1,'e','ew','212','323@qw.com','eweweweewee'),('321232','666','lzw123',3,'null','null','13211232345','ss@123.com','aa'),('3214433','????','123',0,'null','null','18678965432','aa@123.com','adsfaf'),('9087654','???ˮ','123',2,'null','null','15612344321','ssss@124.com','sadfasf');
/*!40000 ALTER TABLE `teacher` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `usertbl`
--

DROP TABLE IF EXISTS `usertbl`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `usertbl` (
  `NAME` varchar(30) DEFAULT NULL,
  `PASSWORD` varchar(20) DEFAULT NULL,
  `SEX` varchar(10) DEFAULT NULL,
  `PRIVENCE` varchar(20) DEFAULT NULL,
  `AUTHOR` varchar(200) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `usertbl`
--

LOCK TABLES `usertbl` WRITE;
/*!40000 ALTER TABLE `usertbl` DISABLE KEYS */;
INSERT INTO `usertbl` VALUES ('silence','zs123','?','???????','??һ?????');
/*!40000 ALTER TABLE `usertbl` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2016-11-20 23:29:31

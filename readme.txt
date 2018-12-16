安装mysql和mysql-client
安装mysql-workbench

> sudo pacman -S mysql mysql-client mysql-workbench
> sudo mysqld --initialize --user=mysql --basedir=/usr --datadir=/var/lib/mysql
> sudo systemctl start mysqld
> sudo mysqladmin -u root -p _&v4rwLaUS?% password "root"

ubuntu上
> sudo apt install mysql-server mysql-client libmysqlclient-dev mysql-workbench

用workbench 创建 schema,名字为bicycle

用命令行
> mysql -u root -p tony1234
> CREATE DATABASE bicycle
> source /pathto/bicycle.sql

安装mysql8.0或更高版本需要在mysql中运行
> alter user root@localhost identified with mysql_native_password by 'root';

安装tomcat8
> sudo pacman -S tomcat8
> vim /etc/tomcat8/tomcat-users.xml   //更改密码 ubuntu上在/var/lib/tomcat8
> sudo gpasswd -a tony tomcat
> sudo systemctl start tomcat8      //ubuntu sudo servie tomcat8 start
配置https
创建JKS证书
> keytool -genkey -v -alias testKey -keyalg RSA -validity 3650 -keystore ~/testKey.keystore
编辑/etc/tomcat8/server.xml，将下面部分取消注释，添加路径和口令     //ubuntu在/var/lib/tomcat8/conf/
    <Connector port="8443" protocol="org.apache.coyote.http11.Http11Protocol"
               maxThreads="150" SSLEnabled="true" scheme="https" secure="true"
               clientAuth="false" sslProtocol="TLS" keystoreFile="你的keystore路径" keystorePass="生成证书时的口令"  />


IDEA配置tomcat服务器
tomcat_home 填写 /usr/share/tomcat8
ubuntu 上填写/var/lib/tomcat8
-- MySQL dump 10.10
--
-- Host: testing    Database: gluxi
-- ------------------------------------------------------
-- Server version	5.0.22-Debian_3-log

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
-- Table structure for table `aliases`
--

DROP TABLE IF EXISTS `aliases`;
CREATE TABLE `aliases` (
  `plugin` tinyint(4) NOT NULL,
  `storage` tinyint(4) NOT NULL,
  `name` varchar(50) collate utf8_bin NOT NULL,
  `value` varchar(200) collate utf8_bin NOT NULL,
  PRIMARY KEY  (`plugin`,`storage`,`name`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_bin;

--
-- Table structure for table `conference_alists`
--

DROP TABLE IF EXISTS `conference_alists`;
CREATE TABLE `conference_alists` (
  `conference_id` int(11) NOT NULL,
  `list` tinyint(1) NOT NULL,
  `value` varchar(50) collate utf8_bin NOT NULL,
  `expire` datetime default NULL,
  PRIMARY KEY  (`conference_id`,`list`,`value`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_bin;

--
-- Table structure for table `conference_jids`
--

DROP TABLE IF EXISTS `conference_jids`;
CREATE TABLE `conference_jids` (
  `id` int(10) unsigned NOT NULL auto_increment,
  `conference_id` int(11) NOT NULL,
  `jid` varchar(50) collate utf8_bin NOT NULL,
  `resource` varchar(50) collate utf8_bin default NULL,
  `temporary` tinyint(1) NOT NULL default '0',
  `created` datetime default NULL,
  PRIMARY KEY  (`id`),
  KEY `jid` (`conference_id`,`jid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_bin;

--
-- Table structure for table `conference_nicks`
--

DROP TABLE IF EXISTS `conference_nicks`;
CREATE TABLE `conference_nicks` (
  `id` int(11) NOT NULL auto_increment,
  `conference_id` int(11) NOT NULL,
  `nick` varchar(50) collate utf8_bin NOT NULL,
  `jid` int(11) NOT NULL default '0',
  `created` datetime NOT NULL,
  `online` tinyint(1) NOT NULL default '0',
  `joined` datetime NOT NULL,
  `lastaction` datetime NOT NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_bin;

--
-- Table structure for table `conferences`
--

DROP TABLE IF EXISTS `conferences`;
CREATE TABLE `conferences` (
  `name` varchar(50) collate utf8_bin NOT NULL,
  `id` int(11) NOT NULL auto_increment,
  `nick` varchar(50) collate utf8_bin NOT NULL,
  `created` datetime NOT NULL,
  `autojoin` tinyint(1) NOT NULL default '1',
  `online` tinyint(1) NOT NULL default '0',
  `joined` datetime default NULL,
  PRIMARY KEY  (`name`),
  UNIQUE KEY `id` (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_bin;

--
-- Table structure for table `words`
--

DROP TABLE IF EXISTS `words`;
CREATE TABLE `words` (
  `plugin` tinyint(4) NOT NULL,
  `storage` tinyint(4) NOT NULL,
  `name` varchar(50) collate utf8_unicode_ci NOT NULL,
  `nick` varchar(50) collate utf8_unicode_ci NOT NULL,
  `date` datetime NOT NULL,
  `value` blob NOT NULL,
  PRIMARY KEY  (`plugin`,`storage`,`name`,`nick`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2006-11-30 22:06:52
-- phpMyAdmin SQL Dump
-- version 
-- http://www.phpmyadmin.net
--
-- Время создания: Мар 14 2015 г., 20:22
-- Версия сервера: 5.5.35
-- Версия PHP: 5.4.4-14+deb7u14

SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

-- --------------------------------------------------------

--
-- Структура таблицы `ssn_accounts`
--

CREATE TABLE IF NOT EXISTS `ssn_accounts` (
  `acc_id` smallint(11) NOT NULL,
  `acc_name` varchar(200) NOT NULL,
  `acc_comment` varchar(255) DEFAULT NULL,
  `acc_is_active` tinyint(4) NOT NULL DEFAULT '1',
  `acc_key` varchar(100) DEFAULT NULL
) ENGINE=MyISAM DEFAULT CHARSET=cp1251 COMMENT='accounts information';

-- --------------------------------------------------------

--
-- Структура таблицы `ssn_commands`
--

CREATE TABLE IF NOT EXISTS `ssn_commands` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT,
  `account` smallint(6) NOT NULL,
  `object` tinyint(4) NOT NULL,
  `command` smallint(6) NOT NULL,
  `token` char(16) DEFAULT NULL COMMENT 'used token',
  `time_store` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `state` tinyint(4) NOT NULL DEFAULT '1' COMMENT '0 - draft, 1 - ready, 2 - loaded, 3 - in processing',
  `command_data` varchar(8000) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=cp1251 AUTO_INCREMENT=4 ;

-- --------------------------------------------------------

--
-- Структура таблицы `ssn_logs`
--

CREATE TABLE IF NOT EXISTS `ssn_logs` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT,
  `account` smallint(6) NOT NULL,
  `object` tinyint(4) NOT NULL,
  `record_type` tinyint(6) NOT NULL COMMENT 'type code: 1 - log, 2 - preferences',
  `timestamp` int(16) NOT NULL COMMENT 'used token',
  `time_store` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `record_data` varchar(8000) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=cp1251 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Структура таблицы `ssn_teledata`
--

CREATE TABLE IF NOT EXISTS `ssn_teledata` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT,
  `account` smallint(6) NOT NULL,
  `object` tinyint(4) NOT NULL,
  `sensor` smallint(6) NOT NULL,
  `index` tinyint(4) NOT NULL DEFAULT '0' COMMENT 'sensor value index',
  `time_send` bigint(20) NOT NULL,
  `time_store` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `sensor_value` bigint(20) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `x_ind_timestamp` (`time_store`)
) ENGINE=MyISAM  DEFAULT CHARSET=cp1251 AUTO_INCREMENT=12404 ;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;

-- phpMyAdmin SQL Dump
-- version 4.0.5
-- http://www.phpmyadmin.net
--
-- 主机: localhost
-- 生成日期: 2014 年 01 月 22 日 16:17
-- 服务器版本: 5.5.34-MariaDB
-- PHP 版本: 5.5.8

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";

--
-- 数据库: `coeus`
--
CREATE DATABASE IF NOT EXISTS `coeus` DEFAULT CHARACTER SET utf8 COLLATE utf8_bin;
USE `coeus`;

-- --------------------------------------------------------

--
-- 表的结构 `users`
--

CREATE TABLE IF NOT EXISTS `users` (
  `uid` int(11) NOT NULL AUTO_INCREMENT,
  `passwd` binary(21) NOT NULL,
  `email` varchar(128) COLLATE utf8_bin NOT NULL,
  `reg_date` int(11) NOT NULL,
  `email_verify` tinyint(4) NOT NULL,
  `last_login` int(11) NOT NULL,
  PRIMARY KEY (`uid`),
  UNIQUE KEY `email` (`email`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COLLATE=utf8_bin AUTO_INCREMENT=4 ;

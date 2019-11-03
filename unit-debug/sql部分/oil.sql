-- phpMyAdmin SQL Dump
-- version 4.7.4
-- https://www.phpmyadmin.net/
--
-- Host: localhost
-- Generation Time: 2019-08-24 12:32:14
-- 服务器版本： 5.5.56-MariaDB
-- PHP Version: 7.0.22

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET AUTOCOMMIT = 0;
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `control`
--

-- --------------------------------------------------------

--
-- 表的结构 `oil`
--

CREATE TABLE `oil` (
  `id` bigint(20) NOT NULL,
  `x` double DEFAULT NULL,
  `y` double DEFAULT NULL,
  `h` double DEFAULT NULL,
  `path` text COLLATE utf8mb4_bin,
  `time` timestamp NULL DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_bin;

--
-- 转存表中的数据 `oil`
--

INSERT INTO `oil` (`id`, `x`, `y`, `h`, `path`, `time`) VALUES
(1, 108.94415, 34.300399, 56, '/image/1.jpg', '2019-08-22 16:00:00'),
(2, 108.947403, 34.312546, 56, '/image/2.jpg', '2019-08-22 16:00:01'),
(3, 108.945955, 34.325648, 56, '/image/3.jpg', '2019-08-22 16:00:02'),
(4, 109.00022, 34.316263, 56, '/image/4.jpg', '2019-08-22 16:00:03'),
(5, 108.9460745501, 34.3489639219, 56, '/image/5.jpg', '2019-08-22 16:00:04');

--
-- Indexes for dumped tables
--

--
-- Indexes for table `oil`
--
ALTER TABLE `oil`
  ADD PRIMARY KEY (`id`);

--
-- 在导出的表使用AUTO_INCREMENT
--

--
-- 使用表AUTO_INCREMENT `oil`
--
ALTER TABLE `oil`
  MODIFY `id` bigint(20) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=6;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;

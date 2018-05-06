BEGIN TRANSACTION;
CREATE TABLE IF NOT EXISTS `users` (
        `Id`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
        `Name`	TEXT NOT NULL UNIQUE
);
CREATE TABLE IF NOT EXISTS `user_cards` (
        `Id`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
        `UserId`	INTEGER NOT NULL,
        `CardId`	INTEGER NOT NULL,
        `Difficulty`	REAL NOT NULL,
        `LastReviewed`	TEXT NOT NULL,
        `DaysBetweenReviews`	INTEGER NOT NULL,
        FOREIGN KEY(`CardId`) REFERENCES `cards`(`Id`) ON DELETE CASCADE,
        UNIQUE(`UserId`,`CardId`),
        FOREIGN KEY(`UserId`) REFERENCES `users`(`Id`) ON DELETE CASCADE
);
CREATE TABLE IF NOT EXISTS `review_history` (
        `Id`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
        `DateTime`	TEXT NOT NULL,
        `PerformanceRating`	NUMERIC NOT NULL,
        `DifficultyAfter`	REAL NOT NULL,
        `DaysBetweenReviewsAfter`	INTEGER NOT NULL,
        `UserCardId`	INTEGER NOT NULL
        FOREIGN KEY(`UserCardId`) REFERENCES `user_cards`(`Id`) ON DELETE CASCADE
);
CREATE TABLE IF NOT EXISTS `cards` (
        `Id`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
        `Name`	TEXT NOT NULL,
        `Type`	INTEGER NOT NULL,
        `Data`	TEXT,
        `GroupId`	INTEGER NOT NULL,
        `Created`	TEXT NOT NULL,
        FOREIGN KEY(`GroupId`) REFERENCES `card_groups`(`Id`) ON DELETE CASCADE
);
CREATE TABLE IF NOT EXISTS `card_groups` (
        `Id`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
        `Name`	TEXT NOT NULL,
        `Settings`	TEXT NOT NULL DEFAULT '{}'
);
COMMIT;

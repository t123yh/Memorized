CREATE TABLE `tags` (
        `Id`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
        `Name`	TEXT NOT NULL
);
CREATE TABLE IF NOT EXISTS "tags_cards" (
        `NoteId`	INTEGER NOT NULL,
        `TagId`	INTEGER NOT NULL,
        FOREIGN KEY(`NoteId`) REFERENCES `notes`(`Id`),
        PRIMARY KEY(`NoteId`,`TagId`)
);
CREATE TABLE IF NOT EXISTS "review_history" (
        `Id`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
        `CardId`	INTEGER NOT NULL,
        `DateTime`	TEXT NOT NULL,
        `PerformanceRating`	REAL NOT NULL,
        `DifficultyAfter`	REAL NOT NULL,
        `DaysBetweenReviewsAfter`	INTEGER NOT NULL,
        FOREIGN KEY(`CardId`) REFERENCES `cards`(`Id`) ON DELETE CASCADE
);
CREATE TABLE `card_groups` (
        `Id`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
        `Name`	TEXT NOT NULL,
        `Settings`	TEXT NOT NULL DEFAULT '{}'
);
CREATE TABLE `cards` (
        `Id`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
        `Name`	TEXT NOT NULL,
        `Type`	INTEGER NOT NULL,
        `Data`	TEXT,
        `LastReviewed`	TEXT,
        `DaysBetweenReviews`	INTEGER,
        `Difficulty`	REAL NOT NULL,
        `GroupId`	INTEGER NOT NULL,
        `Created`	TEXT NOT NULL,
        FOREIGN KEY(`GroupId`) REFERENCES `card_groups`(`Id`) ON DELETE CASCADE
);

INSERT INTO `card_groups` (`Name`) VALUES ('Default');

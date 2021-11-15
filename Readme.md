https://user-images.githubusercontent.com/63211297/141737547-e410bc08-7eca-4d94-8e02-a0f88a5418df.mp4

# 개요

실시간 전투에 초점을 맞추고 진행하다보니 아직 RPG는 없습니다.

서로 총알을 쏘고, 피하는 것이 목표입니다.

IOCP로 다중화했고, 데이터베이스를 통해 로그인/로그아웃하고 데이터를 저장합니다.

# 사용언어

C++(14)

# 환경

Windows10

MySQL 필요

# 프로젝트 기간

2021.11.09 ~ 2021.11.14

# 프로젝트 설명

### 서버

```bash
# Release모드로 빌드 후
/x64/Release/Server.exe
```

### 데이터베이스

`player`라는 테이블을 갖는 데이터베이스를 추가해야 합니다.

```sql
CREATE TABLE		`player2`	(
	`idplayer`	INT		NOT	NULL,
	`name`		VARCHAR(45)	NOT	NULL,
	`password`	VARCHAR(45)	NOT	NULL,
	`position_x`	FLOAT		NULL	DEFAULT	3,
	`position_y`	FLOAT		NULL	DEFAULT	3,
	`direction_x`	FLOAT		NULL	DEFAULT	1,
	`direction_y`	FLOAT		NULL	DEFAULT	0,
	`speed`		FLOAT		NULL	DEFAULT	0.08,
	`shape`		CHAR		NULL	DEFAULT	'N',
	PRIMARY	KEY	(`idplayer`));
	
```

`server_config.txt`로 서버의 설정파일을 작성해야 합니다. 형식은 아래와 같습니다.

서버 실행파일과 같은 디렉토리에 있어야 합니다.

```
database_host: 127.0.0.1
database_user: root
database_password: 1234
database_name: rpg_test
port: 9090
```

### 클라이언트

```bash
# Release모드로 빌드 후
/x64/Release/Client.exe
```

조작법

```bash
전진: 방향키(위)
회전: 방향키(좌우)
발사: 스페이스바
```

- GUI환경에서 마우스 입력을 추가하여 방향을 곧바로 변경하는 것을 고려해서 게임 구조를 설계했습니다.

빌드가 곤란하면 아래 파일을 다운로드 하면 됩니다. 하지만 여전히 MySQL이 필요합니다.

[Release.zip](https://s3-us-west-2.amazonaws.com/secure.notion-static.com/8862e054-7d88-4cfc-b7b3-efae399f082d/Release.zip)

### 플레이 설명

총알을 피하고, 상대방을 먼저 죽이는 것이 목표입니다.

### 주의사항

- 클라이언트를 동시에 여러개 실행할 수는 있습니다. 그러나 클라이언트의 키입력 메서드가 워낙 강력해서, 키 하나를 입력하면 모든 클라이언트가 동시에 받아들입니다.
- 동시에 연결가능한 클라이언트의 수에 제한은 없습니다. 그러나 메시지 버퍼의 크기가 유한하므로, 플레이어가 많아지고, 투사체가 많아지면 쉽게 오버플로가 발생할 것입니다.

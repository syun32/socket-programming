# socket_programming
* computer network assignment
* TCP/UDP socket을 사용하여 각각 network program 구현

# UDP socket programming
* 애니메이션 frame 송/수신 및 playing program
* UDP의 unreliable한 특성에 따라 packet 한두개가 손실되어도 재생에 큰 지장이 없는 Streaming multimedia와 같이, PowerShell창에서 출력할 수 있는 문자 frame을 받아 출력하는 program
* emoji.h에 출력할 frame을 추가한다면, 다양한 애니메이션 출력 가능
* server 실행하여 port number 지정
* client 실행하여 server IP/port number를 통해 server에 접근
* server가 client에 메뉴 화면 제공
* client의 메뉴 선택에 따라 server에서 text frame 제공
* client가 exit 전송 시, program 종료

# TCP socket programing
* remote terminal command program
* Remote terminal access Application의 Underlying transport protocol이 TCP라는 것을 상기하며 Client에서 전송한 명령어를 Server에서 원격으로 실행할 수 있는 program을 구현
* IP와 Port number만으로 명령어를 실행할 수 있다면 보안에 극도로 취약할 것이기 때문에, Server에서 설정한 PASSWORD를 입력하고 일치하여야 명령어를 전송할 수 있도록 설계
* UDP program과 동일한 방식으로 server IP/port number를 통해 연결
* client가 입력한 비밀번호가 server에서 설정된 비밀번호와 같아야 command 입력 라인 생성
* client가 전송한 command가 server에서 실행됨 
* client가 exit 전송 시, program 종료

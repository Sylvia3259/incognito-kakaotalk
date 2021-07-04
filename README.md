# Incognito KakaoTalk
윈도우용 카카오톡을 위한 시크릿 창에서 링크 열기 기능입니다.  
## 사용법
1. ```Incognito KakaoTalk.exe``` 파일을 실행합니다. _(시작 프로그램으로 등록해두시면 편리합니다.)_
2. 카카오톡에서 시크릿 창에서 열려는 링크를 왼쪽 ```Ctrl``` + ```Shift``` 키를 누른 상태로 클릭합니다.
   * 만약 환경 설정에서 ```actions``` 설정을 변경하였을 경우 해당 동작을 취하면서 링크를 클릭합니다.
## 환경 설정
Incognito KakaoTalk은 환경 설정 기능을 제공합니다.  
환경 설정은 ```config.json``` 파일의 내용을 수정해 변경할 수 있습니다.
```json
{
    "browser": "default",
    "actions": [
        "LCLS"
    ],
    "domains": [
        {
            "always_open_in_incognito_window": [],
            "never_open_in_incognito_window": []
        }
    ]
}
```
1. browser
   * 어떤 브라우저로 시크릿 창을 열지 설정합니다.
     - ```default``` : 시스템 기본 브라우저로 시크릿 창을 엽니다.
     - ```chrome``` : 구글 크롬 브라우저로 시크릿 창을 엽니다.
     - ```edge``` : 마이크로소프트 엣지 브라우저로 시크릿 창을 엽니다.
     - ```firefox``` : 모질라 파이어폭스 브라우저로 시크릿 창을 엽니다.
     - ```whale``` : 네이버 웨일 브라우저로 시크릿 창을 엽니다.
2. actions
   * 어떤 동작을 취했을 때 링크를 시크릿 창으로 열지 설정합니다.
     - ```LCLS``` : 링크를 왼쪽 ```Ctrl``` + ```Shift``` 키를 누른 상태로 클릭하면 링크를 시크릿 창으로 엽니다.
     - ```LC``` : 링크를 왼쪽 ```Ctrl``` 키를 누른 상태로 클릭하면 링크를 시크릿 창으로 엽니다.
3. domains
   * always_open_in_incognito_window
     - 이 목록에 있는 도메인은 ```actions``` 에서 설정한 동작을 취하지 않더라도 시크릿 창으로 열립니다.
   * never_open_in_incognito_window
     - 이 목록에 있는 도메인은 ```actions``` 에서 설정한 동작을 취더라도 시크릿 창으로 열리지 않습니다.
## 주의사항
* ```Incognito KakaoTalk.exe``` 파일과 ```Payload.dll``` 파일이 같은 디렉토리에 있어야 합니다.
* 환경 설정 기능을 사용하시려면 ```config.json``` 파일도 위 두 파일과 같은 디렉토리에 넣으십시오.
* 지원하는 브라우저 목록은 다음과 같습니다. 적어도 이들 중 하나는 설치되어 있어야 합니다.
  - 구글 크롬
  - 마이크로소프트 엣지
  - 모질라 파이어폭스
  - 네이버 웨일

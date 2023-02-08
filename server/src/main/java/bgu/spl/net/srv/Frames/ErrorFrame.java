package bgu.spl.net.srv.Frames;

import java.util.concurrent.ConcurrentHashMap;
import java.util.List;

public class ErrorFrame {
    private List<String> errorList;
    private ConcurrentHashMap<String,String> headerToValue;

    public ErrorFrame(List<String> errorList, ConcurrentHashMap<String, String> headers) {
        headerToValue = headers;
        this.errorList = errorList;
    }

    @Override
    public String toString() {
        String answer = "ERROR\n";
        if (headerToValue.get("receipt") != null) {
            answer += "receipt :" + headerToValue.get("receipt") + "\n";
        }
        answer += "message :invalid frame recevied\n\n";
        answer += "The Message: \n----- \n";
        answer += headerToValue.get("COMMAND") + "\n";
        for (String key : headerToValue.keySet()) {
            if(key != "COMMAND" && key != "BODY")
                answer += key + " :" + headerToValue.get(key) + "\n";
        }
        answer += "\n" + headerToValue.get("BODY") + "\n----- \n";
        for (String error : errorList) {
            answer += error + "\n";
        }
        answer += "\u0000";
        return answer;
    }
}

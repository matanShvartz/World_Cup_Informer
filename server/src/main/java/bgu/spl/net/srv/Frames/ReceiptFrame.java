package bgu.spl.net.srv.Frames;

public class ReceiptFrame {
    
    public String receiptID;
    public ReceiptFrame(String receiptID) {
        this.receiptID = receiptID;
    }

    @Override
    public String toString() {
        String answer = "RECEIPT\n";
        answer += "receipt - id :" + receiptID.toString() + "\n\n\u0000";
        return answer;
    }
}

package bgu.spl.net.impl.echo;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.net.Socket;

public class EchoClient {

    public static void main(String[] args) throws IOException {

        if (args.length == 0) {
            args = new String[]{"localhost"};
        }

        //BufferedReader and BufferedWriter automatically using UTF-8 encoding
        try (Socket sock = new Socket(args[0], 7777);
                BufferedReader in = new BufferedReader(new InputStreamReader(sock.getInputStream()));
                BufferedWriter out = new BufferedWriter(new OutputStreamWriter(sock.getOutputStream()))) {

            System.out.println("sending CONNECT Frame to server");
            String connectFrame = "CONNECT\naccept - version :1.2\nhost : stomp . cs . bgu . ac . il\nlogin : meni\npasscode : films\n\n\u0000";
            out.write(connectFrame);
            out.flush();
            System.out.println("awaiting response from server. Sleep on socket read");

            // System.out.println("sending SUBSCRIBE Frame to server");
            // String subscribeFrame = "SUBSCRIBE\ndestination :/germany_spain\nid :17\nreceipt :73\n\n\u0000";
            // out.write(subscribeFrame);
            // out.flush();
            // System.out.println("SUBSCRIBE frame was sent.");

            // System.out.println("sending UNSUBSCRIBE Frame to server");
            // String unsubscribeFrame = "UNSUBSCRIBE\nid :17\nreceipt :74\n\n\u0000";
            // out.write(unsubscribeFrame);
            // out.flush();
            // System.out.println("UNSUBSCRIBE frame was sent.");

            // System.out.println("sending DISCONNECT Frame to server");
            // String disconnectFrame = "DISCONNECT\nreceipt :77\n\n\u0000";
            // out.write(disconnectFrame);
            // out.flush();
            // System.out.println("DISCONNECT frame was sent.");
        }
    }
}

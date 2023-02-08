package bgu.spl.net.impl.stomp;

import bgu.spl.net.impl.echo.LineMessageEncoderDecoder;
import bgu.spl.net.srv.Server;
import bgu.spl.net.srv.StompMessagingProtocolImpl;

public class StompServer {

    public static void main(String[] args) {
        if (args.length == 0) {
            args = new String[]{"7777", "tpc"};
        }

        if (args[1].equals("reactor")) {
            System.out.println("Reactor server");
            Server.reactor(
                4,
                Integer.parseInt(args[0]),
                () -> new StompMessagingProtocolImpl<String>(),
                LineMessageEncoderDecoder::new).serve();

        } else if(args[1].equals("tpc")){
            System.out.println("Tpc server");
            Server.threadPerClient(
                Integer.parseInt(args[0]),
                () -> new StompMessagingProtocolImpl<String>(),
                LineMessageEncoderDecoder::new).serve();
        } else {
            System.out.println("Wrong input");
        }
    }
}

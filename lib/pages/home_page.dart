import 'package:flutter/material.dart';


import '../util/smart_device_box.dart';
import 'drawer.dart';

class HomePage extends StatefulWidget {
  const HomePage({super.key});

  @override
  State<HomePage> createState() => _HomePageState();
}

class _HomePageState extends State<HomePage> {
  // padding constants
  final double horizontalPadding = 40;
  final double verticalPadding = 25;

  // list of smart devices
  List mySmartDevices = [
    // [ smartDeviceName, iconPath , powerStatus ]
    ["Smart Light", "lib/icons/light-bulb.png", true],
    ["Smart AC", "lib/icons/air-conditioner.png", true],
    ["Smart TV", "lib/icons/smart-tv.png", true],
    ["Smart Fan", "lib/icons/fan.png", true],
    ["Smart Door Lock", "lib/icons/light-bulb.png", true],
    ["Smart Door ", "lib/icons/smart-tv.png", true],
    ["Smart Light 1", "lib/icons/light-bulb.png", true],
  ];

  // power button switched
  void powerSwitchChanged(bool value, int index) {
    setState(() {
      mySmartDevices[index][2] = value;
    });
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      backgroundColor: Colors.grey[300],
      appBar: AppBar(
        backgroundColor: Colors.grey[300],
        elevation: 0,
        actions: [
          IconButton(
              onPressed: () {},
              icon: Icon(
                Icons.notifications_none,
              ))
        ],
      ),
      drawer: MyDrawer(),
      body: SafeArea(
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            // app bar
            Padding(
              padding: EdgeInsets.symmetric(
                horizontal: horizontalPadding,
                vertical: verticalPadding,
              ),
              // child: Row(
              //   mainAxisAlignment: MainAxisAlignment.spaceBetween,
              //   children: [
              //     // menu icon
              //     Image.asset(
              //       'lib/icons/menu.png',
              //       height: 45,
              //       color: Colors.grey[800],
              //     ),
              //
              //     // account icon
              //     Icon(
              //       Icons.person,
              //       size: 45,
              //       color: Colors.grey[800],
              //     )
              //   ],
              // ),
            ),

            // welcome home
            Padding(
              padding: EdgeInsets.symmetric(horizontal: horizontalPadding),
              child: Column(
                crossAxisAlignment: CrossAxisAlignment.start,
                children: [
                  Text(
                    "Hello Canh,",
                    style: TextStyle(
                        fontSize: 25,
                        color: Colors.grey.shade800,
                        fontWeight: FontWeight.bold),
                  ),
                  Text(
                    'Welcome To Home',
                    // style: GoogleFonts.bebasNeue(fontSize: 25),
                    style: TextStyle(fontSize: 25, color: Colors.grey.shade900),
                  ),
                ],
              ),
            ),
            const SizedBox(height: 15),

            Row(
              children: [
                Expanded(
                    child: Column(
                  crossAxisAlignment: CrossAxisAlignment.start,
                  children: [
                    Padding(
                      padding:
                          EdgeInsets.symmetric(horizontal: horizontalPadding),
                      child: Column(
                        crossAxisAlignment: CrossAxisAlignment.start,
                        children: [
                          Row(children: [
                            Image.asset('lib/icons/temperature.png',width: 40,
                            height: 40,
                            ),
                          ],),
                          Text(
                            "40°c",
                            style: TextStyle(
                              fontSize: 16,
                              fontWeight: FontWeight.w600,
                              color: Colors.grey[800],
                            ),
                          ),
                          Text(
                            "Temperature",
                            style: TextStyle(fontSize: 14, color: Colors.grey[800]),
                          )
                        ],
                      ),
                    )
                  ],
                ),
                ),
                Expanded(
                  child: Column(
                    crossAxisAlignment: CrossAxisAlignment.start,
                    children: [
                      Padding(
                        padding:
                        EdgeInsets.symmetric(horizontal: horizontalPadding),
                        child: Column(
                          crossAxisAlignment: CrossAxisAlignment.start,
                          children: [
                            Row(children: [
                              Image.asset('lib/icons/humidity.png',width: 40,
                                height: 40,
                              ),
                            ],),
                            Text(
                              "59%",
                              style: TextStyle(
                                fontSize: 16,
                                fontWeight: FontWeight.w600,
                                color: Colors.grey[800],
                              ),
                            ),
                            Text(
                              "Humidity",
                              style: TextStyle(fontSize: 14, color: Colors.grey[800]),
                            )
                          ],
                        ),
                      )
                    ],
                  ),
                ),
              ],
            ),

            const SizedBox(height: 20),

            const Padding(
              padding: EdgeInsets.symmetric(horizontal: 40.0),
              child: Divider(
                thickness: 1,
                color: Color.fromARGB(255, 204, 204, 204),
              ),
            ),

            const SizedBox(height: 10),

            // smart devices grid
            Padding(
              padding: EdgeInsets.symmetric(horizontal: horizontalPadding),
              child: Text(
                "Smart Devices",
                style: TextStyle(
                  fontWeight: FontWeight.bold,
                  fontSize: 24,
                  color: Colors.grey.shade800,
                ),
              ),
            ),

            // grid
            Expanded(
              child: GridView.builder(
                itemCount: mySmartDevices.length,
                // physics: const NeverScrollableScrollPhysics(),
                padding: const EdgeInsets.symmetric(horizontal: 15),
                gridDelegate: const SliverGridDelegateWithFixedCrossAxisCount(
                  crossAxisCount: 2,
                  childAspectRatio: 1 / 1.3,
                ),
                itemBuilder: (context, index) {
                  return SmartDeviceBox(
                    smartDeviceName: mySmartDevices[index][0],
                    iconPath: mySmartDevices[index][1],
                    powerOn: mySmartDevices[index][2],
                    onChanged: (value) => powerSwitchChanged(value, index),
                  );
                },
              ),
            )
          ],
        ),
      ),
    );
  }
}

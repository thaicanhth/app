import 'package:app/pages/home_page.dart';
import 'package:flutter/cupertino.dart';
import 'package:flutter/material.dart';

class Next_home extends StatefulWidget {
  const Next_home({super.key});

  @override
  State<Next_home> createState() => _Next_homeState();
}

class _Next_homeState extends State<Next_home> {
  @override
  Widget build(BuildContext context) {
    return Scaffold(
      backgroundColor: Colors.grey[800],
      body: Column(
        children: [
          Padding(
            padding:
                const EdgeInsets.only(top: 60, right: 60, bottom: 30, left: 60),
            child: Image.asset(
              'lib/icons/login.png',
            ),
          ),
          Padding(
            padding: const EdgeInsets.all(24.0),
            child: Text(
              'Sweet & Smart Home',
              textAlign: TextAlign.center,
              style: TextStyle(
                  color: Colors.white,
                  fontSize: 30,
                  fontWeight: FontWeight.bold),
            ),
          ),
          Text(
            'Smart Home can change ',
            style: TextStyle(
                fontSize: 14, fontWeight: FontWeight.bold, color: Colors.grey),
          ),
          Text(
            'way you live in the future ',
            style: TextStyle(
                fontSize: 14, fontWeight: FontWeight.bold, color: Colors.grey),
          ),
          SizedBox(
            height: 40,
          ),
          GestureDetector(
            onTap: () => Navigator.pushReplacement(context,
                MaterialPageRoute(builder: (context) {
              return HomePage();
            })),
            child: Container(
              decoration: BoxDecoration(
                color: Colors.white,
                borderRadius: BorderRadius.circular(12),
              ),
              padding: EdgeInsets.all(24),
              child: Text(
                'Get Started',
              ),
            ),
          ),
        ],
      ),
    );
  }
}

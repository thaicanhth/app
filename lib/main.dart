<<<<<<<<< Temporary merge branch 1
import 'package:app/pages/home_page.dart';
=========

import 'package:app/pages/next_home.dart';
>>>>>>>>> Temporary merge branch 2
import 'package:flutter/material.dart';


void main() {
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  // This widget is the root of your application.
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      debugShowCheckedModeBanner: false,
      title: "Smart Home",
      theme: ThemeData(
        fontFamily: "Poppins",
      ),
<<<<<<<<< Temporary merge branch 1
     home: HomePage(),
=========
     home: Next_home(),
>>>>>>>>> Temporary merge branch 2
    );
  }
}

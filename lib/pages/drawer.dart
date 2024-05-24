import 'package:app/pages/my_list_tile.dart';
import 'package:flutter/material.dart';
import 'package:provider/provider.dart';

import '../providers/user_provider.dart';

class MyDrawer extends StatelessWidget {
  final void Function()? onProfileTap;
  const MyDrawer({
    super.key,
    required this.onProfileTap,
  });

  @override
  Widget build(BuildContext context) {
    final user = Provider.of<UserProvider>(context).user;
    return Drawer(
      backgroundColor: Colors.grey,
      child: ListView(
        padding: EdgeInsets.zero,
        children: [
          UserAccountsDrawerHeader(
            accountName: Text('HI ${user.name}'),
            accountEmail: Text('${user.email}'),
            currentAccountPicture: CircleAvatar(
              child: ClipOval(
                child: Image.asset(
                  'lib/icons/man.jpg',
                ),
              ),
            ),
            decoration: BoxDecoration(
                color: Colors.grey,
                image: DecorationImage(
                  image: AssetImage('lib/icons/home2.jpg'),
                  fit: BoxFit.cover,
                )),
          ),

          MyListTile(
            icon: Icons.home,
            text: 'HOME',
            onTap: () => Navigator.pop(context),
          ),
          MyListTile(
            icon: Icons.person,
            text: 'PROFILE',
            onTap: onProfileTap,
          ),
          MyListTile(
            icon: Icons.bar_chart_outlined,
            text: 'CHAR',
            onTap: onProfileTap,
          ),
        ],
      ),
      //     MyListTile(
      //       icon: Icons.logout_outlined,
      //       text: 'L O G O U T',
      //       onTap: onSignOut,
      //     ),
      //   ],
      // ),
    );
  }
}
